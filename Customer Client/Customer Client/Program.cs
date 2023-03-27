using System;
using System.Collections.Specialized;
using System.Net.Http.Json;
using System.Transactions;

namespace Customer_Client
{
    class Program
    {
        static async Task Main(string[] args)
        {
            int scelta;
            
            do
            {
                Console.WriteLine("\n***MENU CLIENTE***: \n" +
                    " - 1. Visualizza vernici\n" +
                    " - 2. Effettua un ordine\n" +
                    " - Altro. Esci\n" +
                    "Scelta: ");

                if (!int.TryParse(Console.ReadLine(), out scelta))
                {
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
                        Console.WriteLine("Effettuo un ordine...");
                        break;
                    
                    default:
                        Console.WriteLine("\nUscita...");
                        return;
                }
            } while (scelta != 0);
        }

        static async Task VisualizzaVernici()
        {
            var client = new HttpClient();
            var response = await client.GetAsync("http://localhost:8000/vernici");
            var jsonData = await response.Content.ReadFromJsonAsync<JsonData>();

            // json j;

            // for(Vernice v : j["vernici"]) {


            Console.WriteLine("\nVernici in magazzino:");
            foreach (var vernice in jsonData.Vernici)
            {
                //Console.WriteLine($"ID: {vernice.ID}, Nome: {vernice.Nome}, Quantità: {vernice.QuantitaKg} kg, Prezzo: {vernice.PrezzoKg} €/kg");
                vernice.Stampa();

            }
        }
    }
    
    class JsonData
    {
        public Vernice[] Vernici { get; set; }
    }
}

/*
using System;
using System.Net.Http;
using System.Text.Json;
using System.Threading.Tasks;

namespace VerniciApp
{
    class Program
    {
        static async Task Main(string[] args)
        {
            Console.WriteLine("Benvenuto nella VerniciApp!");

            while (true)
            {
                Console.WriteLine("Cosa vuoi fare?");
                Console.WriteLine("1. Visualizza Vernici");
                Console.WriteLine("2. Effettua ordine");
                Console.WriteLine("3. Esci");

                int scelta = int.Parse(Console.ReadLine());

                switch (scelta)
                {
                    case 1:
                        await VisualizzaVernici();
                        break;
                    case 2:
                        await EffettuaOrdine();
                        break;
                    case 3:
                        Console.WriteLine("Arrivederci!");
                        return;
                    default:
                        Console.WriteLine("Scelta non valida, riprova.");
                        break;
                }
            }
        }

        static async Task VisualizzaVernici()
        {
            using var client = new HttpClient();
            var response = await client.GetAsync("http://localhost:8000/vernici");
            response.EnsureSuccessStatusCode();
            var jsonString = await response.Content.ReadAsStringAsync();
            var vernici = JsonSerializer.Deserialize<Vernice[]>(jsonString);

            Console.WriteLine("ID\tNome\tQuantità disponibile\tPrezzo al kg");
            Console.WriteLine("-----------------------------------------------");

            foreach (var vernice in vernici)
            {
                Console.WriteLine($"{vernice.Id}\t{vernice.Nome}\t{vernice.QuantitaDisponibile} kg\t\t{vernice.Prezzo} €/kg");
            }
        }

        static async Task EffettuaOrdine()
        {
            Console.WriteLine("Inserisci l'ID della Vernice che vuoi ordinare:");
            int id = int.Parse(Console.ReadLine());

            Console.WriteLine("Inserisci la quantità di Vernice che vuoi ordinare (in kg):");
            int quantita = int.Parse(Console.ReadLine());

            using var client = new HttpClient();
            var response = await client.GetAsync($"http://localhost:8000/vernici/{id}");
            response.EnsureSuccessStatusCode();
            var jsonString = await response.Content.ReadAsStringAsync();
            var vernice = JsonSerializer.Deserialize<Vernice>(jsonString);

            if (quantita <= vernice.QuantitaDisponibile)
            {
                var ordine = new Ordine
                {
                    IdVernice = id,
                    Quantita = quantita,
                    Stato = "Completato"
                };

                var ordineJson = JsonSerializer.Serialize(ordine);
                var content = new StringContent(ordineJson, System.Text.Encoding.UTF8, "application/json");

                var postResponse = await client.PostAsync("http://localhost:8000/ordini", content);
                postResponse.EnsureSuccessStatusCode();

                vernice.QuantitaDisponibile -= quantita;
                var verniceJson = JsonSerializer.Serialize(vernice);
                var updateContent = new StringContent(verniceJson, System.Text.Encoding.UTF8, "application/json");
                var putResponse = await client.PutAsync($"http://localhost:8000/vernici/{id}", updateContent);
                putResponse.EnsureSuccessStatusCode();

                Console.WriteLine("Ordine completato con successo");
            }
        }
    }
}
*/
