#pragma once

#include "ProdottoIndustriale.h"

class ProdottoVendibile : public ProdottoIndustriale {
protected:
	float prezzoKg;

public: 
	/* Costruttori */
	ProdottoVendibile(string nome, float prezzoKg);
	ProdottoVendibile(string nome, float quantitaKg, float prezzoKg);

	/* Metodi per l'attributo prezzoKg */
	float getPrezzoKg() const;
	void setPrezzoKg(float prezzoKg);

	/* Altri metodi */
	virtual void stampa() const = 0;
	virtual json toJson() const = 0;
	virtual string whoAmI() const = 0;
};

