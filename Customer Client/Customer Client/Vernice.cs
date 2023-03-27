using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Customer_Client
{
    public class Vernice : ProdottoAcquistabile 
    {
        public float PrezzoKg { 
            get { return Prezzo; } 
            set { Prezzo = value; } 
        }

        public float QuantitaKg
        {
            get { return Quantita; }
            set { Quantita = value; }
        }

        public Vernice(int ID, string nome, float quantitaKg, float prezzoKg) : base(ID, nome, quantitaKg, prezzoKg) {
        }

        public override void Stampa()
        {
            Console.WriteLine(ID + ". Vernice " + Nome + " - Q.tà disponibile: " + QuantitaKg + "Kg - " + PrezzoKg + " Euro/Kg");
        }

    }
}
