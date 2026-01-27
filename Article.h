#pragma once
using namespace System;

namespace app {
	public enum Jednostka {
		Sztuka,
		Kilogram
	};

	public ref class ArtykulSpozywczy {

	public:
		property String^ Rodzaj;
		property double Cena;
		property Jednostka Jedn;

		property double Ilosc;

		ArtykulSpozywczy(String^ rodzaj, double cena, Jednostka jedn, double ilosc) {
			Rodzaj = rodzaj;
			Cena = cena;
			Jedn = jedn;
			Ilosc = ilosc;
		}

		double Wartosc() {
			return Cena * Ilosc;
		}
	};
}