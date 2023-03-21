#pragma once 

#include "Componente.h"

class Colore : public Componente {
public:  
	/* Costruttori */
	Colore(string nome);
	Colore(string nome, float quantitaKg);
	Colore(Colore& c);
	Colore(const Componente& c);

	/* Altri metodi */
	void stampa() const override;	// implementazione del metodo astratto
	void whoAmI() const;

	/* Overload operatori */
	Colore& operator=(const Colore& c);
	Colore& operator+=(const float quantitaKg);
	bool operator==(const Colore& c);
};
