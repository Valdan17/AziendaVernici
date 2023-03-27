using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Json;
using System.Text;
using System.Threading.Tasks;

namespace Customer_Client
{
    static class Database
    {
        private static Dictionary<int, Ordine> ordini;
        private static List<Vernice> vernici;

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
            set { vernici = value; }
        }
        

        private static async Task prelevaVernici()
        {
            List<Vernice> vernici = new List<Vernice>();

            var client = new HttpClient();
            var response = await client.GetAsync("http://localhost:8000/vernici");
            var jsonData = await response.Content.ReadFromJsonAsync<JsonData>();

            foreach (var vernice in jsonData.Vernici)
            {
                vernici.Add(vernice);
            }

            Vernici = vernici;
        }

        private static stampaVernici()
        {
            for(Vernice )
        }
    }
}
