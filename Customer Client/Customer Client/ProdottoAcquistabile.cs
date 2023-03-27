using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Customer_Client
{
    public abstract class ProdottoAcquistabile
    {
        private int id;
        private string nome;
        private float quantita, prezzo;

        public int ID
        {
            get { return id; }
            set { id = value; }
        }

        public string Nome
        {
            get { return nome; }
            set { nome = value; }
        }

        public float Quantita
        {
            get { return quantita; }
            set { quantita = value; }
        }

        public float Prezzo
        {
            get { return prezzo; }
            set { prezzo = value; }
        }

        public ProdottoAcquistabile(int ID, string nome, float quantitaKg, float prezzoKg)
        {
            this.id = ID;
            this.nome = nome;
            this.quantita = quantitaKg;
            this.prezzo = prezzoKg;
        }

        public abstract void Stampa();
    }
}
