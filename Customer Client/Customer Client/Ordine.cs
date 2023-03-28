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
        private List<RigaDiVendita<ProdottoAcquistabile>> righeDiVendita;
        private float importoTotale;
        //private bool completato;

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

        public Ordine(List<RigaDiVendita<ProdottoAcquistabile>> righeDiVendita, float importoTotale)
        {
            this.righeDiVendita = righeDiVendita;
            this.importoTotale = importoTotale;
        }

        public void Stampa()
        {
            foreach (RigaDiVendita<ProdottoAcquistabile> righeDiVendita in righeDiVendita)
            {
                righeDiVendita.Stampa();
            }
            Console.WriteLine("Importo totale: " + importoTotale);
        }


    }  
}
