#include<iostream>
#include<vector>
#include<iterator>
#include<thread>
#include<exception>
#include<sstream>
#include<string>
#include<mutex>
#include<regex>
using namespace std;

const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";
mutex m;
enum Sortiranje { ASC, DESC };
enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };
enum Razred { I = 1, II, III, IV };
const char* crt = "-------------------------------------------\n";
const char* razredChar[] = { "I","II","III","IV" };
const char* predmetChar[] = { " MATEMATIKA", "FIZIKA", "HEMIJA", "GEOGRAFIJA", "NOT_SET" };
bool validanFormat(string telefon) {
	return regex_match(telefon, regex("((\\+)(\\d{3})[(\\() ](\\d{2})[\\) ](\\d{3})(\\-)(\\d{3}))"));
}
char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template <class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int _trenutnoElemenata;
	bool _dozvoliDupliranje;
	Sortiranje _sortiranje;
	void kopiraj(const Kolekcija& k) {
		for (int i = 0; i < k._trenutnoElemenata; i++)
			if (k._elementi1[i] != nullptr && k._elementi2[i] != nullptr) {
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._elementi2[i]);
			}
	}
	void sort() {
		bool promjena = true;
		while (promjena) {
			promjena = false;
			for (int i = 0; i < _trenutnoElemenata - 1; i++)
				if (_sortiranje == ASC && (*_elementi1[i]) > (*_elementi1[i + 1])) {
					swap((*_elementi1[i]), (*_elementi1[i + 1]));
					swap((*_elementi2[i]), (*_elementi2[i + 1]));
					promjena = true;
				}
				else if (_sortiranje == DESC && (*_elementi1[i]) < (*_elementi1[i + 1])) {
					swap(*_elementi1[i], *_elementi1[i + 1]);
					swap(*_elementi2[i], *_elementi2[i + 1]);
					promjena = true;
				}
		}
	}

public:
	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true) {
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		_dozvoliDupliranje = dozvoliDupliranje;
	}
	~Kolekcija() {
		for (size_t i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutnoElemenata = obj._trenutnoElemenata;
		_sortiranje = obj._sortiranje;
		_dozvoliDupliranje = obj._dozvoliDupliranje;
		kopiraj(obj);

	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj) {
			_trenutnoElemenata = obj._trenutnoElemenata;
			_sortiranje = obj._sortiranje;
			_dozvoliDupliranje = obj._dozvoliDupliranje;
			for (int i = 0; i < max; i++) {
				if (_elementi1[i] != nullptr)
					delete _elementi1[i];
				if (_elementi2[i] != nullptr)
					delete _elementi2[i];
			}
			kopiraj(obj);
		}
		return *this;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}

	void AddElement(T1 t1, T2 t2) {
		if (_trenutnoElemenata == GetMax())
			throw exception("Kolekcija je popunjena.\n \n");

		for (int i = 0; i < _trenutnoElemenata; i++) {
			if (GetElement1(i) == t1 && GetElement2(i) == t2)
				throw exception("Dupliranje nije moguce\n \n");
		}

		_elementi1[_trenutnoElemenata] = new T1(t1);
		_elementi2[_trenutnoElemenata++] = new T2(t2);
		if (_sortiranje && _trenutnoElemenata > 1)
			sort();

	}
};

class Aktivnost {
	shared_ptr<Razred> _razred;
	string _opis;
	int _ocjena;//1-5
public:
	Aktivnost(Razred razred = I, int ocjena = 0, string opis = not_set) {
		if (ocjena < 1 || ocjena>5)
			throw exception("Ocjena je u rangu od 1-5");

		_ocjena = ocjena;
		_opis = opis;
		_razred = make_shared<Razred>(razred);
	}
	int  GetOcjenu() const { return _ocjena; }
	string GetOpis() const { return _opis; }
	Razred GetRazred() const { return *_razred; }

	friend ostream& operator<<(ostream& COUT, const Aktivnost& obj) {
		COUT << " " << razredChar[*obj._razred] << " " << obj._ocjena << " " << obj._opis << endl;
		return COUT;
	}
	friend bool operator==(const Aktivnost&, const Aktivnost&);
};
bool operator==(const Aktivnost& a1, const Aktivnost& a2) {
	return *a1._razred == *a2._razred && a1._opis == a2._opis && a1._ocjena == a2._ocjena;
}
class Polaznik {
protected:
	char* _imePrezime;
	string _brojTelefona;
public:
	Polaznik(string imePrezime, string brojTelefona) : _imePrezime(AlocirajNizKaraktera(imePrezime.c_str())) {

		if (!validanFormat(brojTelefona))
			brojTelefona = not_set;
		_brojTelefona = brojTelefona;
	}
	Polaznik(const Polaznik& obj) {
		_imePrezime = AlocirajNizKaraktera(obj._imePrezime);
		_brojTelefona = obj._brojTelefona;
	}
	Polaznik& operator=(const Polaznik& obj) {
		if (this != &obj) {
			if (_imePrezime != nullptr)
				delete[]_imePrezime;
			_imePrezime = AlocirajNizKaraktera(obj._imePrezime);
			_brojTelefona = obj._brojTelefona;
		}
		return *this;
	}
	~Polaznik() { delete[] _imePrezime; }
	char* GetImePrezime() { return _imePrezime; }
	string GetTelefon() { return _imePrezime; }
	virtual void PredstaviSe() = 0;
};

class Ucenik :public Polaznik {
	Kolekcija<Predmet, Aktivnost, 16>* _aktivnosti;
public:
	Ucenik(string imeP = "---", string tel = "---") :Polaznik(imeP, tel) {
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>();
	}
	Ucenik(const Ucenik& obj) :Polaznik(obj._imePrezime, obj._brojTelefona),
		_aktivnosti(new Kolekcija<Predmet, Aktivnost, 16>(*obj._aktivnosti)) {}
	Ucenik& operator=(const Ucenik& obj) {
		if (this != &obj) {
			if (_aktivnosti != nullptr)
				delete _aktivnosti;
			_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>(*obj._aktivnosti);
			Polaznik::operator=(obj);
		}
		return *this;
	}
	~Ucenik() { delete _aktivnosti; _aktivnosti = nullptr; }
	Kolekcija<Predmet, Aktivnost, 16>& GetAktivnosti() { return *_aktivnosti; };
	friend ostream& operator<<(ostream& COUT, Ucenik& n);
	virtual void PredstaviSe() {
		cout << crt << "Ime i prezime: " << Polaznik::_imePrezime;
		cout << "\nBroj telefona: " << Polaznik::_brojTelefona;
		cout << "\n--LISTA AKTIVNOSTI--\n";
		if (_aktivnosti->GetTrenutno() == 0)
			cout << "-->TRENUTNO NEMA AKTIVNOSTI<--" << crt;
		else {
			for (int i = 0; i < _aktivnosti->GetTrenutno(); i++) {
				cout << "Predmet: " << predmetChar[_aktivnosti->GetElement1(i)];
				cout << _aktivnosti->GetElement2(i) << endl << endl;
			}
		}

	}
	Ucenik& operator()(Predmet p, const  Aktivnost& a) {
		_aktivnosti->AddElement(p, a);
		return *this;
	}
	double GetProsjecnuOcjenu() {
		double ocj = 0;
		for (int i = 0; i < _aktivnosti->GetTrenutno(); i++)
			ocj += _aktivnosti->GetElement2(i).GetOcjenu();
		return ocj / _aktivnosti->GetTrenutno();
	}
};
ostream& operator<<(ostream& cout, Ucenik& n) {
	n.PredstaviSe();
	return cout;
}
class Skola {
	char* _naziv;
	vector<Ucenik> _ucenici;

	void posaljiSMS(string ime, Razred r, float ocj) {
		cout << crt;
		cout << "SEND SMS TO: " << ime << endl << crt;
		this_thread::sleep_for(chrono::seconds(3));
		cout << "Uspjesno ste okoncali aktivnost u okviru " << razredChar[r - 1] << " razreda sa prosjecnom ocjenom " << ocj << crt;
		this_thread::sleep_for(chrono::seconds(2));
	}
public:
	Skola(const char* naziv = nullptr) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Skola() {
		delete[] _naziv; _naziv = nullptr;
	}
	Skola(const Skola& obj) {
		_naziv = AlocirajNizKaraktera(obj._naziv);
		_ucenici = obj._ucenici;
	}
	Skola& operator=(const Skola& obj) {
		if (this != &obj) {
			if (_naziv = nullptr)
				delete[]_naziv;
			_naziv = AlocirajNizKaraktera(obj._naziv);
			_ucenici = obj._ucenici;
		}
		return *this;
	}
	char* GetNaziv()const { return _naziv; }
	vector<Ucenik>& GetUcenici() { return _ucenici; };
	Ucenik* GetUcenik(string ime) {
		for (int i = 0; i < _ucenici.size(); i++) {
			if (_ucenici[i].GetImePrezime() == ime)
				return &_ucenici[i];
		}return nullptr;
	}
	friend ostream& operator<<(ostream& COUT, Skola& obj) {
		COUT << "Skola: " << obj._naziv << endl;
		COUT << "Ucenici: " << endl;
		for (size_t i = 0; i < obj._ucenici.size(); i++)
			COUT << obj._ucenici[i] << endl;
		return COUT;
	}
	Skola& operator()(string imePrezime, string telefon) {
		for (vector<Ucenik>::iterator i = _ucenici.begin(); i != _ucenici.end(); i++) {
			if (i->GetImePrezime() == imePrezime)
				throw exception("Ucenik s tim imenom i prezimenom vec postoji");
			if (i->GetTelefon() == telefon)
				throw exception("Ucenik s tim brojem telefona postoji");
		}
		_ucenici.push_back(Ucenik(imePrezime, telefon));
		return *this;
	}

	bool DodajAktivnost(string imePrezime, Predmet p, const Aktivnost& a) {

		Ucenik& ucenik = *GetUcenik(imePrezime);
		float prosjek = 0;
		int brojac = 0;
		if (GetUcenik(imePrezime) == nullptr)
			return false;
		for (int i = 0; i < ucenik.GetAktivnosti().GetTrenutno(); i++) {
			if (a.GetRazred() == ucenik.GetAktivnosti().GetElement2(i).GetRazred()) {
				brojac++;
				prosjek += ucenik.GetAktivnosti().GetElement2(i).GetOcjenu();

				if (p == ucenik.GetAktivnosti().GetElement1(i))
					return false;
			}
		}
		if (brojac > 4)
			return false;
		prosjek /= brojac;
		ucenik(p, a);
		if (brojac == 3 && prosjek > 1 && ucenik.GetTelefon() != "NOT_SET") {
			m.lock();
			thread t(&Skola::posaljiSMS, this, imePrezime, a.GetRazred(), prosjek);
			t.join();
			m.unlock();
		}
		return true;
	}

	pair<Polaznik*, float>GetNajboljegUcenika() {
		vector<double>ocjena;
		for (int i = 0; i < _ucenici.size(); i++)
			ocjena.push_back(_ucenici[i].GetProsjecnuOcjenu());

		int najbolji = 0;
		for (int i = 1; i < ocjena.size(); i++)
			if (ocjena[i] > ocjena[najbolji])
				najbolji = i;

		return make_pair<Polaznik*, float>(&_ucenici[najbolji], (float)ocjena[najbolji]);
	}
};

int main() {
	Kolekcija<int, int, 10> kolekcija1(DESC, false);
	try {
		kolekcija1.AddElement(1, 2);
		//dupliranje elemenata nije dozvoljeno
		kolekcija1.AddElement(1, 2);
	}
	catch (exception & ex) {
		cout << ex.what();
	}

	/*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/

	for (size_t i = 1; i < kolekcija1.GetMax() - 1; i++)
		kolekcija1.AddElement(rand(), rand());


	cout << kolekcija1 << endl;

	try {
		//prekoracen maksimalan broj elemenata
		kolekcija1.AddElement(rand(), rand());
	}
	catch (exception & ex) {
		cout << ex.what();
	}
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;

	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");
	//dodaje novog ucenika u skolu
	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");
	gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/
	gimnazijaMostar("Telefon NotValidFormat", "387 61)333-444");

	try
	{
		/*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/
		gimnazijaMostar("Adel Handzic", "+387(61)333-444");
	}
	catch (exception & ex)
	{
		cout << ex.what() << endl;
	}

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti, a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))
		cout << "Aktivnost nije uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda, te posjeduje validan broj telefona,
	u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa prosjecnom ocjenom X.X"*/
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))
		cout << "Aktivnost uspjesno dodana" << endl;

	//ispisuje sve ucenike i njihove aktivnosti
	cout << gimnazijaMostar << endl;


	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();
	cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;


	system("pause>0");
	return 0;
}
