using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Headers;
using System.Net.Http.Json;
using Newtonsoft.Json;
using System.Runtime.ConstrainedExecution;
using System.Text;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace Customer_Client
{
    static class Database
    {
        private static Dictionary<int, Ordine> ordini;
        private static List<Vernice> vernici = new List<Vernice>();

        /*
        public Dictionary<int, Ordine> ordini
        {
            get { return preleva 
            }
        }
        */

        public static List<Vernice> Vernici
        {
            get { return vernici; }
        }

        public static async Task<string> get(string url)
        {
            using (var client = new HttpClient())
            {
                var response = await client.GetAsync(url);
                var jsonResponse = await response.Content.ReadAsStringAsync();

                return jsonResponse;

            }
        }

        public static async Task PrelevaVernici()
        {
            vernici.Clear();

            var jsonResponse = await get("http://localhost:8000/vernici");

            // Analizza il JSON e estrae la lista di vernici
            var result = JsonConvert.DeserializeObject<Dictionary<string, List<Dictionary<string, object>>>>(jsonResponse);
            var jsonVernici = (List<Dictionary<string, object>>)result["vernici"];

            // Scansiona ogni vernice nel JSON e crea un nuovo oggetto Vernice da salvare nella lista delle vernici
            foreach (var jsonVernice in jsonVernici)
            {
                var vernice = new Vernice(Convert.ToInt32(jsonVernice["ID"]), (string)jsonVernice["nome"], Convert.ToSingle(jsonVernice["quantitaKg"]), Convert.ToSingle(jsonVernice["PrezzoKg"]));
                vernici.Add(vernice);
            }
        }

        /*
        public static async Task PrelevaVernici()
        {
            vernici.Clear();

            using (var client = new HttpClient())
            {
                var response = await client.GetAsync("http://localhost:8000/vernici");
                var jsonResponse = await response.Content.ReadAsStringAsync();

                // Analizza il JSON e estrae la lista di vernici
                var result = JsonConvert.DeserializeObject<Dictionary<string, List<Dictionary<string, object>>>>(jsonResponse);
                var jsonVernici = (List<Dictionary<string, object>>)result["vernici"];

                // Scansiona ogni vernice nel JSON e crea un nuovo oggetto Vernice da salvare nella lista delle vernici
                foreach (var jsonVernice in jsonVernici)
                {
                    var vernice = new Vernice(Convert.ToInt32(jsonVernice["ID"]), (string)jsonVernice["nome"], Convert.ToSingle(jsonVernice["quantitaKg"]), Convert.ToSingle(jsonVernice["PrezzoKg"]));
                    vernici.Add(vernice);
                }
            }
        }
        */

        public static async Task VisualizzaVernici()
        {
            if (vernici.Count > 0)
            {
                Console.WriteLine("\nVernici in vendita: ");
                foreach (Vernice vernice in vernici)
                {
                    vernice.Stampa();
                }
            }
            else
            {
                Console.WriteLine("\nNon ci sono vernici in magazzino\n");
            }
        }

        public static async Task StampaVernici()
        {
            try
            {
                await PrelevaVernici();
            }
            catch (HttpRequestException ex)
            {
                Console.WriteLine("\nSi è verificato un errore durante la richiesta GET:\n - " + ex.Message);
                return;
            }

            if (vernici.Count > 0)
            {
                Console.WriteLine("\nVernici in vendita: ");
                foreach (Vernice vernice in vernici)
                {
                    vernice.Stampa();
                }
            }
            else
            {
                Console.WriteLine("\nNon ci sono vernici in magazzino\n");
            }
        }
    }
}
