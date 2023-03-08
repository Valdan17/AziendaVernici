#pragma once

#include "ProdottoIndustriale.h"

class Componente : public ProdottoIndustriale {
public:
	/* Costruttori */
	Componente(string nome);
	Componente(string nome, float quantitaKg);
	Componente(Componente& p);

	/* Altri metodi */
	virtual void stampa() const = 0;	// VIRTUALE
	virtual json toJson() const;
	void whoAmI() const;

	/* Overload operatori */
	Componente& operator=(const Componente& c);

	Componente& operator+=(const float quantitaKg);
};

