#include <iostream>
#include <string>
#include <ios>
#include <limits>
#include <type_traits>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "Colore.h"
#include "Additivo.h"
#include "Vernice.h"
#include "Giacenze.h"

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

/*
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    ((std::string*)userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}
*/

/*
template<typename T>
void stampa(map<int, T>& prodotti) {
    static_assert(std::is_base_of<ProdottoIndustriale, T>::value, "Il tipo non � correttamente convertibile");
    for (auto& element : prodotti) {
        cout << element.first;
        element.second->stampa();
    }
}
*/

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

void stampa(map<int, Additivo>& additivi) {
    for (auto& element : additivi) {
        cout << " " << element.first;
        element.second.stampa();
    }
}

void stampa(map<int, Colore>& colori) {
    for (auto& element : colori) {
        cout << " " <<element.first;
        element.second.stampa();
    }
}

void stampa(map<int, Vernice>& vernici) {
    for (auto& element : vernici) {
        cout << " " << element.first;
        element.second.stampa();
    }
}


// effettua una richiesta di GET all'url specificato e ne restituisce il json
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

// effettua una richiesta di POST all'url specificato del json passato 
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

// effettua una richiesta PUT all'url specificato del json passato
void put(string url, json j)
{
    // Inizializza l'handle di curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Errore: impossibile inizializzare l'handle di curl" << std::endl;
        return;
    }

    // Imposta l'URL della richiesta
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Imposta il tipo di richiesta a PUT
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

    // Converte i dati in formato JSON in una stringa
    string data_str = j.dump();

    // Imposta i dati della richiesta
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_str.c_str());

    // Esegue la richiesta
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "Errore: impossibile eseguire la richiesta: " << curl_easy_strerror(res) << std::endl;
    }

    // Rilascia l'handle di curl
    curl_easy_cleanup(curl);
}


/*
void put(string url, json data) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Convert JSON data to string
    std::string dataStr = data.dump();

    // Initialize curl
    curl = curl_easy_init();
    if (curl)
    {
        // Set the URL of the endpoint to send the PUT request to
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the HTTP method to PUT
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        // Set the data to send in the request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataStr.c_str());

        // Set the callback function to receive the response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);

        // Set the buffer to receive the response data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up curl
        curl_easy_cleanup(curl);

        // Print the response data
        std::cout << "Response data: " << readBuffer << std::endl;
    }
}
*/

/* COMPONENTI */


/* ADDITIVI */
map<int, Additivo> deserializzaAdditivi(json j) {
    map<int, Additivo> additivi;

    for (const auto& additivo : j["additivi"]) {
        Additivo a(additivo["nome"], additivo["quantitaKg"]);
        additivi.emplace(additivo["ID"], a);    // serve la funzione emplace() perch� altrimenti servirebbe il costr. vuoto
    }

    return additivi;
}

map<int, Additivo> prelevaAdditivi() {
    string url = "http://localhost:8000/additivi";
    json jsonAdditivi = get(url);

    return deserializzaAdditivi(jsonAdditivi);
}

void visualizzaAdditivi() {
    map<int, Additivo>& additivi = Giacenze::additivi;

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
map<int, Colore> deserializzaColori(json j) {
    map<int, Colore> colori;
    
    for (const auto& colore : j["colori"]) {
        Colore c(colore["nome"], colore["quantitaKg"]);
        colori.emplace(colore["ID"], c);    // serve la funzione emplace() perch� altrimenti servirebbe il costr. vuoto
    }

    return colori;
}

map<int, Colore> prelevaColori() {
    string url = "http://localhost:8000/colori";
    json jsonColori = get(url);

    return deserializzaColori(jsonColori);
}

void visualizzaColori() {
    map<int, Colore>& colori = Giacenze::colori;

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
Vernice deserializzaVernice(json vernice) {
    map<int, Colore>& colori = Giacenze::colori = prelevaColori();
    map<int, Additivo>& additivi = Giacenze::additivi = prelevaAdditivi();
    vector<elementoFormula> formula;

        for (const auto& colore : vernice["colori"]) {
            elementoFormula elem;
            elem.idComponente = colore["IDColore"];
            elem.percentuale = colore["Percentuale"];
            elem.componente = &colori.find(elem.idComponente)->second;
            formula.push_back(elem);
        }

        for (const auto& additivo : vernice["additivi"]) {
            elementoFormula elem;
            elem.idComponente = additivo["IDAdditivo"];
            elem.percentuale = additivo["Percentuale"];
            elem.componente = &additivi.find(elem.idComponente)->second;
            formula.push_back(elem);
        }
        Vernice v(vernice["nome"], vernice["quantitaKg"], vernice["PrezzoKg"], formula);
        return v;
}

map<int, Vernice> deserializzaVernici(json j) {
    map<int, Vernice> vernici;

    for (const auto& vernice : j["vernici"]) {
        vernici.emplace(vernice["ID"], deserializzaVernice(vernice));
    }
   
    return vernici;
}
*/

map<int, Vernice> deserializzaVernici(json j) {
    map<int, Colore>& colori = Giacenze::colori = prelevaColori();
    map<int, Additivo>& additivi = Giacenze::additivi = prelevaAdditivi();
    map<int, Vernice> vernici;

    for (const auto& vernice : j["vernici"]) {
        vector<elementoFormula> formula;

        for (const auto& colore : vernice["colori"]) {
            elementoFormula elem;
            elem.idComponente = colore["IDColore"];
            elem.percentuale = colore["Percentuale"];
            elem.componente = &colori.find(elem.idComponente)->second;
            formula.push_back(elem);
        }

        for (const auto& additivo : vernice["additivi"]) {
            elementoFormula elem;
            elem.idComponente = additivo["IDAdditivo"];
            elem.percentuale = additivo["Percentuale"];
            elem.componente = &additivi.find(elem.idComponente)->second;
            formula.push_back(elem);
        }
            Vernice v(vernice["nome"], vernice["quantitaKg"], vernice["PrezzoKg"], formula);
            vernici.emplace(vernice["ID"], v);
    }
    return vernici;
}

map<int, Vernice> prelevaVernici() {
    string url = "http://localhost:8000/vernici";
    json jsonVernici = get(url);

    return deserializzaVernici(jsonVernici);
}

void visualizzaVernici() {
    map<int, Vernice>& vernici = Giacenze::vernici = prelevaVernici();

    if (vernici.size() > 0) {
        cout << "\nVernici in magazzino: " << endl;
        stampa(vernici);
    }
    else
        cout << "Non sono presenti vernici nel magazzino\n";
}

void inserisciVernice() {
    string nomeVernice, inserimentoPrezzoKg, inserimento, inserimentoPercentuale;
    float prezzoKg, percentualeTotaleComponenti = 0.0f;
    elementoFormula elemento;
    vector<elementoFormula> formula;
    map<int, Colore>& colori = Giacenze::colori = prelevaColori();
    map<int, Additivo>& additivi = Giacenze::additivi = prelevaAdditivi();
    int idComponente;

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
    cout << " - Prezzo di vendita al Kg: ";

    try {
        cin >> inserimentoPrezzoKg;
        prezzoKg = stof(inserimentoPrezzoKg);
    } catch (const exception& e) {
        cout << "Errore: quantita' non valida" << endl;
    }

    pulisciSchermo();

    do {
        cout << "\nFORMULA ATTUALE: " << percentualeTotaleComponenti << "%" << endl;
        if (formula.size() > 0) {
            for (elementoFormula elementoFormula: formula) {
                cout << " - " << elementoFormula.percentuale << "% " << elementoFormula.componente->getNome() << endl;
            }
        }
        cout << "\n1. Aggiungi un colore\n" <<
            "2. Aggiungi un additivo\n";
        cin >> inserimento;
        if (inserimento == "1") {
            visualizzaColori();
            cout << "\nInserisci l'ID del colore da aggiungere: ";
            cin >> idComponente;
            pulisciSchermo();
            if (colori.contains(idComponente)) {
                colori.find(idComponente)->second.stampa();

                for (int i = 0; i < formula.size(); i++) {
                    if (static_cast<Colore>(*formula[i].componente) == colori.find(idComponente)->second) {
                        percentualeTotaleComponenti -= formula[i].percentuale;
                        formula.erase(formula.begin() + i);
                        break;
                    }
                }

                cout << "\nInserisci la % del colore nella formula (0-100%): ";
                cin >> inserimentoPercentuale;
                pulisciSchermo();
                try {
                    elemento.percentuale = stof(inserimentoPercentuale);
                    if (elemento.percentuale <= 0.0) throw exception();
                }
                catch (const exception& e) {
                    cout << "Errore: quantita' non valida" << endl;
                    continue;
                }
                elemento.idComponente = idComponente;
                elemento.componente = &colori.find(idComponente)->second;
                if (elemento.percentuale + percentualeTotaleComponenti > 100) {
                    cout << "\nATTENZIONE: la percentuale inserita supera il totale, impostata al massimo consentito" << endl;
                    elemento.percentuale = 100.0f - percentualeTotaleComponenti;
                }

                percentualeTotaleComponenti += stof(inserimentoPercentuale);
                formula.push_back(elemento);
            }
            else {
                cout << "Il colore selezionato non esiste" << endl;
            }

        } else if(inserimento == "2") {
            visualizzaAdditivi();
            cout << "\nInserisci l'ID dell'additivo da aggiungere: ";
            cin >> idComponente;
            pulisciSchermo();
            if (additivi.contains(idComponente)) {
                additivi.find(idComponente)->second.stampa();

                for (int i = 0; i < formula.size(); i++) {
                    if (static_cast<Additivo>(*formula[i].componente) == additivi.find(idComponente)->second) {
                        percentualeTotaleComponenti -= formula[i].percentuale;
                        formula.erase(formula.begin() + i);
                        break;
                    }
                }

                cout << "\nInserisci la % dell'additivo nella formula (0-100%): ";
                cin >> inserimentoPercentuale;
                pulisciSchermo();
                try { 
                    elemento.percentuale = stof(inserimentoPercentuale);
                    if (elemento.percentuale <= 0.0) throw exception();
                }
                catch (const exception& e) {
                    cout << "Errore: quantita' non valida" << endl;
                    continue;
                }
                elemento.idComponente = idComponente;
                elemento.componente = &additivi.find(idComponente)->second;
                if (elemento.percentuale + percentualeTotaleComponenti > 100) {
                    cout << "\nATTENZIONE: la percentuale inserita supera il totale consentito, impostata al massimo di default" << endl;
                    elemento.percentuale = 100.0f - percentualeTotaleComponenti;
                }
                
                percentualeTotaleComponenti += stof(inserimentoPercentuale);
                formula.push_back(elemento);
            }
            else
                cout << "L'additivo selezionato non esiste" << endl;
        }
        else {
            pulisciSchermo();
            cout << "Scelta non valida\n";
        }
    } while (percentualeTotaleComponenti < 100);

    cout << "\nFormula completa al 100%:" << endl;
    for (elementoFormula elementoFormula : formula) {
        cout << " - " << elementoFormula.percentuale << "% " << elementoFormula.componente->getNome() << endl;
    }
    cout << "\nInserisci 1 per confermare la creazione della vernice, altrimenti per uscire" << endl;
    cin >> inserimento;
    if (inserimento == "1") {
        Vernice v(nomeVernice, prezzoKg, formula);
        cout << v.toJson();
        post("http://127.0.0.1:8000/vernici", v.toJson());
    }
    pulisciSchermo();
}

void visualizzaComposizioneVernice() {
    map<int, Vernice>& vernici = Giacenze::vernici = prelevaVernici();
    int idVernice;
    json jsonVernice;

    cout << "\nVernici in magazzino: " << endl;
    stampa(vernici);
    cout << "\nInserisci l'ID della vernice di cui visualizzare la composizione: ";
    cin >> idVernice;

    pulisciSchermo();

    if (vernici.contains(idVernice)) {
        vernici.find(idVernice)->second.stampaFormula();
    } else {
        cout << "\nVernice non valida" << endl;
    }
}

void aggiornaQuantitaVernice() {
    string idVernice, quantitaKgDaProdurre, confermaProduzione;
    map<int, Vernice>& vernici = Giacenze::vernici = prelevaVernici();
    json j, componente, componenti;

    cout << "\nVernici in magazzino: " << endl;
    stampa(vernici);
    cout << "\nInserisci l'ID della vernice da produrre: ";
    cin >> idVernice;

    pulisciSchermo();

    if (vernici.contains(stoi(idVernice))) {
        Vernice& verniceDaProdurre = vernici.find(stoi(idVernice))->second;
        verniceDaProdurre.stampaFormulaConQuantitaKg();
        
        cout << "\nInserisci la quantita in Kg da produrre: ";
        cin >> quantitaKgDaProdurre;

        pulisciSchermo();
       
        cout << "\nPer la produzione di " << quantitaKgDaProdurre << "Kg di vernice " << verniceDaProdurre.getNome() << ":" << endl;

        if (verniceDaProdurre.isProducibileInQuantitaKg(stof(quantitaKgDaProdurre))) {
            cout << "\nInserire 1 per confermare, altrimenti per annullare ed uscire: ";
            cin >> confermaProduzione;

            pulisciSchermo();

            if (confermaProduzione == "1") {
                for (elementoFormula elementoFormula : verniceDaProdurre.getFormula()) {
                    componente["ID"] = elementoFormula.idComponente;
                    componente["QuantitaKg"] = elementoFormula.componente->getQuantitaKg() - stoi(quantitaKgDaProdurre);
                    componenti.push_back(componente);
                }
                j["Vernice"]["ID"] = idVernice;
                j["Vernice"]["QuantitaKg"] = verniceDaProdurre.getQuantitaKg() + stoi(quantitaKgDaProdurre);
                j["Componenti"] = componenti;
                put("http://127.0.0.1:8000/vernici", j);
            }
            else
                cout << "\nProduzione annullata" << endl;
        }
    }
    else {
        cout << "\nVernice non valida" << endl;
    }
    
}

int main() {
    string input;
    int scelta;

    do {
        cout << "\n***MENU AMMINISTRATORE***\n" <<
            "1. Visualizza colori in magazzino\n"
            "2. Visualizza additivi in magazzino\n"
            "3. Visualizza vernici in magazzino\n"
            "4. Visualizza composizione vernice\n"
            "5. Inserisci nuovo colore in magazzino\n"
            "6. Inserisci nuovo additivo in magazzino\n"
            "7. Inserisci nuova vernice in magazzino\n"
            "8. Produzione di vernice\n"
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
            visualizzaColori();
            break;

        case 2: // Visualizza additivi
            visualizzaAdditivi();
            break;

        case 3: // Visualizza vernici
            visualizzaVernici();
            break;

        case 4:
            visualizzaComposizioneVernice();
            break;

        case 5: // Inserisci nuovo Colore
            inserisciColore();
            break;

        case 6: // Inserisci nuovo additivo
            inserisciAdditivo();
            break;

        case 7: // Inserisci nuova vernice
            inserisciVernice();
            break;

        case 8: // Aggiorna quantit� vernice
            aggiornaQuantitaVernice();
            break;

        default:
            cout << "Esco...\n" << endl;
            break;
        }
    } while (scelta < 9);
}