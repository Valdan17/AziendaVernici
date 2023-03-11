#pragma once

#include "Componente.h"

class Additivo : public Componente {
public: 
	/* Costruttori */
	Additivo(string nome);
	Additivo(string nome, float quantitaKg);
	Additivo(Additivo& a);

	/* Altri metodi */
	virtual void stampa() const override;	// implementazione del metodo astratto

	void whoAmI() const;
};

