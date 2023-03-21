#pragma once

#include "Componente.h"

class Additivo : public Componente {
public: 
	/* Costruttori */
	Additivo(string nome);
	Additivo(string nome, float quantitaKg);
	Additivo(Additivo& a);
	Additivo(Componente& c);

	/* Altri metodi */
	virtual void stampa() const override;	// implementazione del metodo astratto
	void whoAmI() const;

	/* Overload operatori */
	Additivo& operator=(const Additivo& c);
	Additivo& operator+=(const float quantitaKg);
	bool operator==(const Additivo& c);
};

