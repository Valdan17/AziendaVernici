#include <iostream>
#include <string>
#include <ios>
#include <limits>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "Colore.h"
#include "Additivo.h"
#include "Vernice.h"

using json = nlohmann::json;
using namespace std;

struct data {
    char* response;
    size_t size;
};

static size_t cb(void* data, size_t size, size_t nmemb, void* clientp)
{
    size_t realsize = size * nmemb;
    struct data* mem = (struct data*)clientp;

    char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

/* UTILITY */
void pulisciSchermo() {
    cout << "\033[2J\033[1;1H";
}    

void cleanBuffer() {
#pragma push_macro("max")
#undef max
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#pragma pop_macro("max")
}

void stampa(map<int, ProdottoIndustriale*>& elements) {
    for (auto& element : elements) {
        cout << element.first;
        element.second->stampa();
    }
}

// fa una richiesta di GET all'url passato e ne restituisce il json
json get(string url) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    struct data readBuffer = { 0 };

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&readBuffer);
        res = curl_easy_perform(curl);

        json j = json::parse(readBuffer.response);
        return j;
    }
}   

// fa una richiesta di POST all'url passato del json passato 
void post(string url, json j) {
    string body;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Errore nell'inizializzazione di cURL" << std::endl;
        return;
    }

    body = j.dump();

    // Imposta l'header della richiesta
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Imposta la richiesta cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());

    // Esegue la richiesta
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "Errore nella richiesta cURL: " << curl_easy_strerror(res) << endl;
    }

    // Pulisce
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}

/* COMPONENTI */



/* ADDITIVI */
map<int, ProdottoIndustriale*> deserializzaAdditivi(json j) {
    map<int, ProdottoIndustriale*> additivi;

    for (const auto& additivo : j["additivi"]) {
        Additivo* a = new Additivo(additivo["nome"], additivo["quantitaKg"]);
        additivi.emplace(additivo["ID"], a);    // serve la funzione emplace() perché altrimenti servirebbe il costr. vuoto
    }

    return additivi;
}

map<int, ProdottoIndustriale*> prelevaAdditivi() {
    string url = "http://localhost:8000/additivi";
    json jsonAdditivi = get(url);

    return deserializzaAdditivi(jsonAdditivi);
}

void stampaAdditivi() {
    map<int, ProdottoIndustriale*> additivi;

    additivi = prelevaAdditivi();
    if (additivi.size() > 0) {
        cout << "\nAdditivi in magazzino: " << endl;
        stampa(additivi);
    } else
        cout << "Non sono presenti additivi nel magazzino\n";
}

void inserisciAdditivo() {
    string nomeAdditivo, inserimentoQuantitaKg, confermaInserimentoAdditivo;
    float quantitaKg;
    Additivo* nuovoAdditivo;
    json jsonNuovoAdditivo;
    string url = "http://localhost:8000/additivi"; // Imposta l'URL di destinazione

    pulisciSchermo();

    // Crea l'oggetto Additivo
    cout << "\nInserimento di un nuovo additivo: " << endl;
    cout << " - Nome: ";
    cleanBuffer();
    getline(cin, nomeAdditivo);
    if (nomeAdditivo == "") {
        cout << "Il campo nome non deve essere vuoto" << endl;
        return;
    }
    cout << " - Kg: ";
    cin >> inserimentoQuantitaKg;

    try {
        quantitaKg = stof(inserimentoQuantitaKg);
    }
    catch (const invalid_argument& e) {
        pulisciSchermo();
        cout << "Errore: quantita' non valida" << endl;
        return;
    }

    cout << "\nRiepilogo: " <<
        "\n - Colore: " << nomeAdditivo <<
        "\n - Quantita' Kg: " << quantitaKg <<
        "\n Premi 1 per confermare, altrimenti per annullare l'inserimento: ";
    cin >> confermaInserimentoAdditivo;

    pulisciSchermo();

    if (confermaInserimentoAdditivo == "1") {
        nuovoAdditivo = new Additivo(nomeAdditivo, quantitaKg);
        jsonNuovoAdditivo = nuovoAdditivo->toJson();
        post(url, jsonNuovoAdditivo);
        delete nuovoAdditivo;
    }
    else
        cout << "Inserimento annullato\n";
}


/* COLORI */
map<int, ProdottoIndustriale*> deserializzaColori(json j) {
    map<int, ProdottoIndustriale*> colori;
    ProdottoIndustriale* p;
    
    for (const auto& colore : j["colori"]) {
        Colore* c = new Colore(colore["nome"], colore["quantitaKg"]);
        colori.emplace(colore["ID"], c);    // serve la funzione emplace() perché altrimenti servirebbe il costr. vuoto
    }

    return colori;
}

map<int, ProdottoIndustriale*> prelevaColori() {
    string url = "http://localhost:8000/colori";
    json jsonColori = get(url);

    return deserializzaColori(jsonColori);
}

void stampaColori() {
    map<int, ProdottoIndustriale*> colori;

    colori = prelevaColori();
    if (colori.size() > 0) {
        cout << "\nColori in magazzino: " << endl;
        stampa(colori);
    }
    else
        cout << "Non sono presenti colori nel magazzino\n";
}

void inserisciColore() {
    string nomeColore, inserimentoQuantitaKg, confermaInserimentoColore;
    float quantitaKg;
    Colore* nuovoColore;
    json jsonNuovoColore;
    string url = "http://localhost:8000/colori"; // Imposta l'URL di destinazione
    
    pulisciSchermo();
   
    // Crea l'oggetto Colore
    cout << "\nInserimento di un nuovo colore: " << endl;
    cout << " - Nome: ";
    cleanBuffer();
    getline(cin, nomeColore);
    if (nomeColore == "") {
        cout << "Il campo nome non deve essere vuoto" << endl;
        return;
    }
    cout << " - Kg: ";
    cin >> inserimentoQuantitaKg;

    try {
        quantitaKg = stof(inserimentoQuantitaKg);
    } catch (const invalid_argument& e) {
        pulisciSchermo();
        cout << "Errore: quantita' non valida" << endl;
        return;
    }

    cout << "\nRiepilogo: " <<
        "\n - Colore: " << nomeColore <<
        "\n - Quantita' Kg: " << quantitaKg <<
        "\n Premi 1 per confermare, altrimenti per annullare l'inserimento: ";
    cin >> confermaInserimentoColore;

    pulisciSchermo();

    if (confermaInserimentoColore == "1") {
        nuovoColore = new Colore(nomeColore, quantitaKg);
        jsonNuovoColore = nuovoColore->toJson();
        post(url, jsonNuovoColore);
        delete nuovoColore;
    } else 
        cout << "Inserimento annullato\n";
}


/* VERNICI */

/*
TO DO 
    Creare un oggetto vernice con la relativa mappa di elementi e lato server
    riuscire a deserializzarla, scrivendo nella tabella Vernici e Composizione
*/
void prelevaVernici() {
    
}

void inserisciVernice() {
    string nomeVernice, inserimentoQuantitaKg, inserimentoAltroComponente, tipoComponente;
    float quantitaKg;
    vector<elementoFormula> formula;
    pulisciSchermo();

    // Crea l'oggetto Vernice
    cout << "\nInserimento di una nuova vernice: " << endl;
    cout << " - Nome: ";
    cleanBuffer();
    getline(cin, nomeVernice);
    if (nomeVernice == "") {
        cout << "Il campo nome non deve essere vuoto" << endl;
        return;
    }
    
    cout << "Elaborazione della formula:" << endl;
    do {
        cout << "1. Aggiungi un additivo\n" <<
            "2. Aggiungi un colore\n";
        cin >> tipoComponente;
        if (tipoComponente == "1") {
            cout << "Additivo" << endl;

        } else if(tipoComponente == "2") {
            cout << "Colore" << endl;
        }
        else {
            cout << "Scelta non valida\n";
        }
        cout << "Inserisci 1 per inserire un nuovo componente, altrimenti per terminare la formula" << endl;
        cin >> inserimentoAltroComponente;
    } while (inserimentoAltroComponente == "1");
}

int main() {
    string input;
    int scelta;

    do {
        cout << "\n***MENU AMMINISTRATORE***\n" <<
            "1. Visualizza colori in magazzino\n"
            "2. Visualizza additivi in magazzino\n"
            "3. Visualizza vernici in magazzino\n"
            "4. Inserisci nuovo colore in magazzino\n"
            "5. Inserisci nuovo additivo in magazzino\n"
            "6. Inserisci nuova vernice in magazzino\n"
            "Altro. Esci" << endl;

        cin >> input;

        try {
            scelta = stoi(input);
        }
        catch (const invalid_argument& e) {
            cout << "Errore: scelta inserita non valida" << endl;
            return 1;
        }

        pulisciSchermo();

        switch (scelta) {
        case 1: // Visualizza colori
            stampaColori();
            break;

        case 2: // Visualizza additivi
            stampaAdditivi();
            break;

        case 3: // Visualizza vernici
            prelevaVernici();
            break;

        case 4: // Inserisci nuovo Colore
            inserisciColore();
            break;

        case 5: // Inserisci nuovo additivo
            inserisciAdditivo();
            break;

        case 6: // Inserisci nuova vernice
            inserisciVernice();
            break;
        default:
            cout << "Esco...\n" << endl;
            break;
        }
    } while (scelta < 7);
}