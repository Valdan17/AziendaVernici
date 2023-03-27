using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Customer_Client
{
    public class RigaDiVendita<TProdotto> where TProdotto : ProdottoAcquistabile
    {
        private float quantita;
        private float importo;
        private TProdotto prodotto;

        public float QuantitaKg {
            get {return quantita; }
            set { quantita = value;}
        }

        public float Importo
        {
            get { return importo;}
            set { importo = value;}
        }

        public TProdotto Prodotto {
            get { return prodotto; }
            set { prodotto = value; }
        }

        public RigaDiVendita(TProdotto prodotto, float quantita)
        {
            this.prodotto = prodotto;
            this.quantita = quantita;
            importo = prodotto.Prezzo * quantita;
        }
    }

    internal class Ordine
    {
        private List<RigaDiVendita<ProdottoAcquistabile>> righeDiVendita;
        private float importoTotale;
        private bool completato;

        public List<RigaDiVendita<ProdottoAcquistabile>> RigheDiVendita {
            get { return righeDiVendita; }
            set { righeDiVendita = value; }
        }

        public Ordine(List<RigaDiVendita<ProdottoAcquistabile>> righeDiVendita, float importoTotale, bool completato)
        {
            this.righeDiVendita = righeDiVendita;
            this.importoTotale = importoTotale;
            this.completato = completato;
        }
    }  
}
