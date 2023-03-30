using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Customer_Client
{
    internal class Ordine
    {
        private string cliente;
        private List<RigaDiVendita<ProdottoAcquistabile>> righeDiVendita;
        private float importoTotale;

        public string Cliente
        {
            get { return cliente; } 
            private set { cliente = value; }
        }

        public List<RigaDiVendita<ProdottoAcquistabile>> RigheDiVendita 
        {
            get { return righeDiVendita; }
            private set { righeDiVendita = value; }
        }

        public float ImportoTotale
        {
            get { return importoTotale; }
            private set { importoTotale = value; }
        }

        public Ordine(string cliente, List<RigaDiVendita<ProdottoAcquistabile>> righeDiVendita, float importoTotale)
        {
            this.cliente = cliente;
            this.righeDiVendita = righeDiVendita;
            this.importoTotale = importoTotale;
        }

        public void Stampa()
        {
            foreach (RigaDiVendita<ProdottoAcquistabile> righeDiVendita in righeDiVendita)
            {
                righeDiVendita.Stampa();
            }
            Console.WriteLine("Importo totale: " + importoTotale + " Euro");
        }


    }  
}
