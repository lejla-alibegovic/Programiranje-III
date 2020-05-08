#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <regex>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;
const char* crt = "\n---------------------------------------\n";
mutex m;
char* AlocirajNizKaraktera(const char* nesto)
{
	if (nesto == nullptr)
		return nullptr;
	int size = strlen(nesto) + 1;
	char* temp = new char[size];
	strcpy_s(temp, size, nesto);
	return temp;
}
class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);

	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;

	}
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minuti = new int(*obj._minuti);
	}
	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj) {
			if (_dan != nullptr)
				delete _dan;
			_dan = new int(*obj._dan);
			if (_mjesec != nullptr)
				delete _mjesec;
			_mjesec = new int(*obj._mjesec);

			if (_godina != nullptr)
				delete _godina;
			_godina = new int(*obj._godina);
			if (_sati != nullptr)
				delete _sati;
			if (_minuti != nullptr)
				delete _minuti;
			_sati = new int(*obj._sati);
			_minuti = new int(*obj._minuti);
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, DatumVrijeme& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
		return COUT;
	}
	bool operator==(const DatumVrijeme& obj) {
		return *_dan == *obj._dan && *_mjesec == *obj._mjesec && *_godina == *obj._godina && *_sati == *obj._sati;
	}
};

template<class T1, class T2 = int>
class FITKolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija& obj) {
		_trenutno = obj._trenutno;
		_elementi1 = new T1[obj._trenutno];
		_elementi2 = new T2[obj._trenutno];
		for (int i = 0; i < obj._trenutno; i++) {

			_elementi1[i] = obj._elementi1[i];
			_elementi2[i] = obj._elementi2[i];
		}
	}
	FITKolekcija& operator=(const FITKolekcija& obj) {
		if (this != &obj) {
			delete[]_elementi1;
			delete[]_elementi2;
			_trenutno = obj._trenutno;
			_elementi1 = new T1[obj._trenutno];
			_elementi2 = new T2[obj._trenutno];
			for (int i = 0; i < obj._trenutno; i++) {

				_elementi1[i] = obj._elementi1[i];
				_elementi2[i] = obj._elementi2[i];
			}
		}
		return *this;
	}

	~FITKolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	T1* GetT1()const { return _elementi1; }
	T2* GetT2()const { return _elementi2; }
	int GetTrenutno() const { return _trenutno; }

	friend ostream& operator<< (ostream& COUT, FITKolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}

	void Dodaj(T1 t1, T2 t2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[]_elementi1;
		delete[]_elementi2;
		temp1[_trenutno] = t1;
		temp2[_trenutno] = t2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutno++;
	}
	void Sortiraj() {
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (int i = 0; i < _trenutno - 1; i++) {
				if (_elementi1[i] > _elementi1[i + 1]) {
					T1 t1 = _elementi1[i];
					T2 t2 = _elementi2[i];
					_elementi1[i] = _elementi1[i + 1];
					_elementi1[i + 1] = t1;
					_elementi2[i] = _elementi2[i + 1];
					_elementi2[i + 1] = t2;
					promjena = true;
				}
			}
		}
	}
	void DodajNaLokaciju(T1 t1, T2 t2, int lok) {
		if (lok<0 || lok>_trenutno)
			return;
		Dodaj(t1, t2);
		swap(_elementi1[_trenutno - 1], _elementi1[lok]);
		swap(_elementi2[_trenutno - 1], _elementi2[lok]);

	}
	FITKolekcija<T1, T2>* GetJedinstveni() {
		FITKolekcija<T1, T2>* obj = new FITKolekcija<T1, T2>;
		bool dodaj = false;
		for (int i = 0; i < _trenutno; i++) {
			for (int j = 0; j < obj->GetTrenutno(); j++) {
				if (obj->GetT1()[j] == _elementi1[i] && obj->GetT2()[j] == _elementi2[i])
					dodaj = true;
			}
			if (!dodaj)
				obj->Dodaj(_elementi1[i], _elementi2[i]);
		}

		return obj;
	}
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };

class Dokument {
	vrstaDokumenta _vrsta;
	string _naziv;
	string _sadrzaj;
	int _brojStranica;
public:
	Dokument() { _brojStranica = 0; }
	Dokument(vrstaDokumenta vrsta, string naziv) {
		_naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
	}
	string GetNaziv() { return _naziv; };
	string GetSadrzaj() { return _sadrzaj; };
	vrstaDokumenta GetVrsta() { return _vrsta; };
	void UvecajBrojStranica() { _brojStranica++; }
	int GetBrojStranica() { return _brojStranica; }
	friend ostream& operator<<(ostream& COUT, Dokument& obj) {
		COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
		return COUT;
	}
	void DodajSadrzaj(string sadrzaj) {
		_sadrzaj += sadrzaj;
		_brojStranica = _sadrzaj.length() / 30;
	}
};
class Uredjaj {
protected:
	char* _proizvodjac;
	char* _model;
public:
	Uredjaj(const char* proizvodjac, const char* model) {
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
	}
	Uredjaj(const Uredjaj& obj) {
		_proizvodjac = AlocirajNizKaraktera(obj._proizvodjac);
		_model = AlocirajNizKaraktera(obj._model);
	}
	Uredjaj& operator=(const Uredjaj& obj) {
		if (this != &obj) {
			if (_proizvodjac != nullptr) {
				delete[]_proizvodjac;
				_proizvodjac = nullptr;
			}
			_proizvodjac = AlocirajNizKaraktera(obj._proizvodjac);
			if (_model != nullptr) {
				delete[]_model;
				_model = nullptr;
			}
			_model = AlocirajNizKaraktera(obj._model);
		}
		return *this;
	}
	~Uredjaj() {
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, Uredjaj& obj) {
		COUT << obj._proizvodjac << "." << obj._model << endl;
		return COUT;
	}
};
void ispisi(string text) {
	lock_guard<mutex>guard(m);
	cout << text << endl;
}
class Printer :public Uredjaj {
	FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
	vector<string> _zabranjeneRijeci;
public:
	Printer(const char* proizvodjac, const char* model) :Uredjaj(proizvodjac, model) {}
	Printer(const Printer& p) :Uredjaj(p), _printaniDokumenti(p._printaniDokumenti), _zabranjeneRijeci(p._zabranjeneRijeci)
	{}
	FITKolekcija<DatumVrijeme, Dokument>& GetPrintaniDokumenti() { return _printaniDokumenti; };
	vector<string>& GetZabranjeneRijeci() { return _zabranjeneRijeci; };
	void DodajZabranjenuRijec(string rijec) {
		for (vector<string>::iterator i = _zabranjeneRijeci.begin(); i != _zabranjeneRijeci.end(); i++)
			if (*i == rijec)return;
		_zabranjeneRijeci.push_back(rijec);

	}
	void Printaj(DatumVrijeme dv, Dokument dok) {
		_printaniDokumenti.Dodaj(dv, dok);
		if (!regex_match(dok.GetNaziv(), regex("([A-Za-z]{5,15})(\\.)(doc|txt|pdf|html)")))
			throw exception("Naziv nije u ispravnom formatu");
		for (int i = 0; i < _zabranjeneRijeci.size(); i++)
			if (dok.GetSadrzaj().find(_zabranjeneRijeci[i]) != string::npos) //string::npos ->not found
				throw exception("Postoje zabranjene rijeci");


		thread t(ispisi, dok.GetSadrzaj());
		t.detach();

	}

	int GetProsjecanBrojStranicaPoDatumu(DatumVrijeme dv) {
		float ukupno = 0;
		float printanih = 0;
		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++) {
			if (_printaniDokumenti.GetT1()[i] == dv) {
				bool printaj = true;
				if (!regex_match(_printaniDokumenti.GetT2()[i].GetNaziv(), regex("([A-Za-z]{5,15})(\\.)(doc|txt|pdf|html)")))
					printaj = false;

				for (vector<string>::iterator j = _zabranjeneRijeci.begin(); j != _zabranjeneRijeci.end(); j++) {
					if (_printaniDokumenti.GetT2()[i].GetSadrzaj().find(*j) != string::npos)
						printaj = true;
					break;
				}
				if (printaj) {
					ukupno += _printaniDokumenti.GetT2()[i].GetBrojStranica();
					printanih = _printaniDokumenti.GetT2()[i].GetBrojStranica();
				}
				else
					ukupno += _printaniDokumenti.GetT2()[i].GetBrojStranica();

			}
		}
		if (ukupno > 0)
			return printanih / ukupno;
		return 0;
	}
	string GetTopZabranjenuRijec() {
		int brojacNaj = 0;
		int trenutnaIt = 0;
		vector<string>::iterator it = _zabranjeneRijeci.begin();
		for (vector<string>::iterator i = _zabranjeneRijeci.begin(); i != _zabranjeneRijeci.end(); i++) {
			trenutnaIt = 0;
			for (int j = 0; j < _printaniDokumenti.GetTrenutno(); j++) {
				if (_printaniDokumenti.GetT2()[j].GetSadrzaj().find(*i) != string::npos)
					trenutnaIt++;
			}
			if (trenutnaIt > brojacNaj) {
				brojacNaj = trenutnaIt;
				it = i;
			}
		}
		return *it;
	}

	void UkloniDokumente() {
		FITKolekcija<DatumVrijeme, Dokument>f;
		bool ukloni = false;
		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++) {
			if (!regex_match(_printaniDokumenti.GetT2()[i].GetNaziv(), regex("([A-Za-z]{5,15})(\\.)(doc|txt|pdf|html)")))
				ukloni = true;

			for (vector<string>::iterator j = _zabranjeneRijeci.begin(); j != _zabranjeneRijeci.end(); j++) {
				if (_printaniDokumenti.GetT2()[i].GetSadrzaj().find(*j) != string::npos) {
					ukloni = true;
					break;
				}
			}
			if (!ukloni)
				f.Dodaj(_printaniDokumenti.GetT1()[i], _printaniDokumenti.GetT2()[i]);
		}
		_printaniDokumenti = f;
	}
	friend ostream& operator<<(ostream& o, Printer& p) {
		o << *dynamic_cast<Uredjaj*>(&p) << endl;
		o << "Printani dokumenti" << endl;
		for (size_t i = 0; i < p._printaniDokumenti.GetTrenutno(); i++)
			o << p._printaniDokumenti.GetT2()[i].GetNaziv() << endl;
		o << "Zabranjene rijeci: " << endl;
		for (vector<string>::iterator i = p._zabranjeneRijeci.begin(); i != p._zabranjeneRijeci.end(); i++)
			o << *i << endl;
		return o;
	}
};


void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URAÐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOCNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.
	****************************************************************************/

#pragma region TestiranjeDatumVrijeme
	DatumVrijeme danas(1, 2, 2017, 10, 15);
	DatumVrijeme sutra(danas);
	DatumVrijeme prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region FITKolekcija
	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//SORTIRANJE CLANOVA KOLEKCIJE SE VRŠI U RASTUCEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
	brojevi.Sortiraj();
	cout << brojevi << crt;
	//BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
	brojevi.DodajNaLokaciju(v109, v6, 2);
	cout << brojevi << crt;
	brojevi.Sortiraj();
	cout << brojevi << crt;
	/*METODA GetJedinstveni VRACA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.*/
	FITKolekcija<int, int>* jedinstveni = brojevi.GetJedinstveni();
	cout << *jedinstveni << crt;
	*jedinstveni = brojevi;
	cout << *jedinstveni << crt;

	Dokument ispitPRIII(DOC, "ispitPRIII.doc");
	//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUCUJUCI I RAZMAKE
	ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO RE");
	cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
	Dokument ispitBaze(DOC, "ispitBaze.doc");
	ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT");
	cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU

	Printer hp3200("HP", "3200");
	//PRINTER NECE DOZVOLITI PRINTANJE DOKUMENATA U CIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJECI
	hp3200.DodajZabranjenuRijec("RE");// :)
	hp3200.DodajZabranjenuRijec("RAT");
	hp3200.DodajZabranjenuRijec("UBITI");
	hp3200.DodajZabranjenuRijec("RE");// ONEMOGUCITI PONAVLJANJE ZABRANJENIH RIJECI, KORISTITI ITERATORE
	hp3200.DodajZabranjenuRijec("MRZITI");
	try
	{
		/*
		DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
		1. NAZIV DOKUMENTA MORA BITI U SLJEDECEM FORMATU npr: ispitPRIII.doc
		NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE
		BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
		2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEC
		UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA
		ODGOVARAJUCOM PORUKOM, ALI CE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti.
		UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI
		SADRŽAJ DOKUMENTA KOJI SE PRINTA

		*/
		hp3200.Printaj(danas, ispitPRIII);
		hp3200.Printaj(danas, ispitBaze);

		/*
		IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
		VODITI RACUNA O DIJELJENJU ZAJEDNICKIH RESURSA
		*/

	}
	catch (exception & err)
	{
		cout << err.what() << endl;
	}

	//KOD POREÐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;

	Printer hp4000(hp3200);
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
	hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
	cout << hp4000 << crt;

#pragma endregion

}
