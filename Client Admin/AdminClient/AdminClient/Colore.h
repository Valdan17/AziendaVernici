#pragma once 

#include "Componente.h"

class Colore : public Componente {
public:  
	/* Costruttori */
	Colore(string nome);
	Colore(string nome, float quantitaKg);
	Colore(Colore& c);

	/* Altri metodi */
	virtual void stampa() const;	// implementazione del metodo astratto

	void whoAmI() const;
};
