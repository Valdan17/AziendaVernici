using Newtonsoft.Json;
using System;
using System.Collections.Specialized;
using System.Dynamic;
using System.Net.Http.Json;
using System.Text;
using System.Transactions;
using System.Xml.Resolvers;

namespace Customer_Client
{
    class Program
    {
        static string usernameUtenteLoggato;

        static async Task<HttpResponseMessage> Post(string url, string json)
        {
            using var client = new HttpClient();
            var content = new StringContent(json, Encoding.UTF8, "application/json");
            var response = await client.PostAsync(url, content);
            return response;
        }

        static async Task<string> Get(string url)
        {
            using (var client = new HttpClient())
            {
                var response = await client.GetAsync(url);
                var jsonResponse = await response.Content.ReadAsStringAsync();

                return jsonResponse;

            }
        }

        static async Task Menu()
        {
            int scelta;

            while (true)
            {
                Console.Write("\n***MENU CLIENTE***: \n" +
                    " - 1. Visualizza vernici\n" +
                    " - 2. Effettua un ordine\n" +
                    " - Altro. Esci\n" +
                    "Scelta: ");

                if (!int.TryParse(Console.ReadLine(), out scelta))
                {
                    Console.Clear();
                    Console.WriteLine("\nScelta non valida.");
                    continue;
                }

                Console.Clear();

                switch (scelta)
                {
                    case 1:
                        await VisualizzaVernici();
                        break;

                    case 2:
                        await EffettuaOrdine();
                        break;

                    default:
                        Console.WriteLine("\nUscita...");
                        return;
                }
            }
        }

        static async Task<bool> Autentica()
        {
            //bool isAutenticato = true;
            Dictionary<string, string> credenziali = new Dictionary<string, string>();

            Console.Write("\nUsername: ");
            string username = Console.ReadLine();
            Console.Write("Password: ");
            string password = "";
            while (true)
            {
                ConsoleKeyInfo key = Console.ReadKey(true);
                if (key.Key == ConsoleKey.Enter)
                    break;
                password += key.KeyChar;
                Console.Write("*");
            }

            Console.Clear();
            credenziali.Add("Username", username);
            credenziali.Add("Password", password);

            var response = await Post("http://localhost:8000/clienti", JsonConvert.SerializeObject(credenziali));

            if (response.IsSuccessStatusCode)
            {
                usernameUtenteLoggato = username;
                var responseJson = await response.Content.ReadAsStringAsync();
                return bool.Parse(responseJson);
            }
            else
            {
                throw new Exception();
            }
        }

        static async Task Main(string[] args)
        {
            while (true)
            {
                try
                {
                    bool isAutenticato = await Autentica();

                    if (isAutenticato)
                    {
                        Console.WriteLine("Benvenuto " + usernameUtenteLoggato);
                        await Menu();
                    }
                    else
                    {
                        Console.WriteLine("\nCredenziali errate");
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("\nErrore di connessione al server");
                }
            }

        }

        static async Task VisualizzaVernici()
        {
            await Database.StampaVernici();
        }

        static async Task EffettuaOrdine()
        {
            int idVerniceDaOrdinare;
            float quantitaInKgDaOrdinare, importoTotale = 0;
            string ripetiInserimento;
            Vernice verniceDaOrdinare;
            List<RigaDiVendita<ProdottoAcquistabile>> righeDiVendita = new List<RigaDiVendita<ProdottoAcquistabile>>();
            Ordine ordineDaEffettuare;

            await Database.PrelevaVernici();

            if (Database.Vernici.Count > 0)
            {
                do
                {
                    Console.Clear();

                    if (righeDiVendita.Count > 0)
                    {
                        Console.WriteLine("\nOrdine attuale: " + importoTotale + " Euro");
                        foreach (RigaDiVendita<ProdottoAcquistabile> rigaDiVendita in righeDiVendita)
                        {
                            rigaDiVendita.Stampa();
                        }
                    }

                    await Database.VisualizzaVernici();
                    Console.Write("\nInserisci l'ID della vernice da ordinare: ");
                    if (int.TryParse(Console.ReadLine(), out idVerniceDaOrdinare))
                    {
                        Console.Clear();
                        verniceDaOrdinare = Database.Vernici.Find(vernice => vernice.ID == idVerniceDaOrdinare);
                        if (Database.Vernici.Contains(verniceDaOrdinare))
                        {
                            Console.WriteLine("\nHai scelto la vernice: ");
                            verniceDaOrdinare.Stampa();
                            Console.Write("\nInserisci la quantita' in Kg da ordinare: ");
                            if (float.TryParse(Console.ReadLine(), out quantitaInKgDaOrdinare))
                            {
                                RigaDiVendita<ProdottoAcquistabile> r = new RigaDiVendita<ProdottoAcquistabile>(verniceDaOrdinare, quantitaInKgDaOrdinare);
                                Console.WriteLine("\nRiepilogo: ");
                                r.Stampa();
                                Console.Write("\nInserisci 1 per confermare: ");
                                if (Console.ReadLine().Equals("1"))
                                {
                                    // Controllo se quella vernice è stata già inserita e nel caso la sostituisco
                                    if (righeDiVendita.Exists(riga => riga.Prodotto is Vernice && riga.Prodotto == verniceDaOrdinare))
                                    {
                                        importoTotale -= (righeDiVendita.Find(riga => riga.Prodotto == verniceDaOrdinare)).Importo;
                                        righeDiVendita.Remove(righeDiVendita.Find(riga => riga.Prodotto == verniceDaOrdinare));
                                    }

                                }
                                righeDiVendita.Add(r);
                                importoTotale += r.Importo;
                            }
                            else
                            {
                                Console.WriteLine("La quantita' inserita non è valida\n");

                            }
                        }
                        else
                        {
                            Console.WriteLine("\nVernice non esistente");
                        }

                    }
                    else
                    {
                        Console.WriteLine("\nScelta non valida.");
                    }

                    Console.Write("\nPremi 1 per scegliere un altro prodotto: ");
                    ripetiInserimento = Console.ReadLine();

                } while (ripetiInserimento.Equals("1"));

                Console.Clear();
                if (righeDiVendita.Count > 0)
                {
                    ordineDaEffettuare = new Ordine(righeDiVendita, importoTotale);
                    Console.WriteLine("Riepilogo ordine: ");
                    ordineDaEffettuare.Stampa();
                    Console.Write("\nInserisci 1 per confermare l'ordine, altrimenti per uscire: ");
                    if (Console.ReadLine().Equals("1"))
                    {
                        string json = JsonConvert.SerializeObject(ordineDaEffettuare);
                        Console.WriteLine("json che invio e': " + json);

                        var response = await Post("http://localhost:8000/ordini", json);
                        Console.WriteLine("Response che ricevo e': " + response);

                        if (response.IsSuccessStatusCode)
                        {
                            var responseJson = await response.Content.ReadAsStringAsync();
                            Console.WriteLine("Response che converto in string e': " + responseJson);
                        }
                        else
                        {
                            throw new Exception();
                        }

                        Console.WriteLine(response);
                    }
                }
                Console.WriteLine("Termine ordine...");

            }
            else
            {
                Console.WriteLine("Impossibile effettuare ordini al momento");
            }
        }
    }
}