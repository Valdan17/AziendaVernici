using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json.Nodes;
using System.Threading.Tasks;

namespace Customer_Client
{
    public class RigaDiVendita<TProdotto> where TProdotto : ProdottoAcquistabile
    {
        private float quantita;
        private float importo;
        private TProdotto prodotto;

        public float QuantitaKg
        {
            get { return quantita; }
            set { quantita = value; }
        }

        public float Importo
        {
            get { return importo; }
            set { importo = value; }
        }

        public TProdotto Prodotto
        {
            get { return prodotto; }
            set { prodotto = value; }
        }

        public RigaDiVendita(TProdotto prodotto, float quantita)
        {
            this.prodotto = prodotto;
            this.quantita = quantita;
            importo = prodotto.PrezzoKg * quantita;
        }

        public void Stampa()
        {
            Console.WriteLine(" - " + prodotto.tipoProdotto + " " + prodotto.Nome + ", " + QuantitaKg + "Kg, " + Importo + " Euro");
        }
    }

}
