#include <bits/stdc++.h>
using namespace std;

ostringstream izhod[2];
int tI = 0;

struct sprem {
  int pol;
  int glob;
};

struct funkcija {
  int pol;
  int stArgumentov;
};

map<string, sprem> spre;
map<string, funkcija> funk;
vector<string> lokSpre;
vector<string> glob = {"r15", "rbp"};
vector<int> stSpre = {1, 1};

int stVrstice = 0;
void napaka(string &s, int &ind, string sporocilo) {
  cout << "Vrstica " << stVrstice << ":" << endl;
  cout << s << endl;
  for (int i = 0; i < s.size(); i++) {
    if (i == ind)
      cout << "^";
    else
      cout << "~";
  }
  cout << endl << sporocilo << endl;
}

void ign(string &s, int &ind) {
  while (isspace(s[ind]))
    ind++;
}

string dobiZeton(string &s, int &ind) {
  ign(s, ind);
  string zeton = "";
  while (isalpha(s[ind]) || (zeton != "" && isdigit(s[ind]))) {
    zeton += s[ind++];
  }
  return zeton;
}

void shraniSpremenljivko(string ime) {
  spre[ime] = (sprem){stSpre[tI]++, tI};
  if (tI) {
    lokSpre.push_back(ime);
  }
}

void definirajSpremenljivko(string ime) {
  izhod[tI] << "\tpush 0" << endl;
  shraniSpremenljivko(ime);
}

void stevilo(string &s, int &ind) {
  ign(s, ind);
  string st = "";

  while ('0' <= s[ind] && s[ind] <= '9') {
    st += s[ind];
    ind++;
  }
  izhod[tI] << "\tmov rax, " << st << endl;
}

void spremenljivka(string &s, int &ind) {
  string zeton = dobiZeton(s, ind);
  if (!spre.count(zeton)) {
    napaka(s, ind, "Nedefinirana spremenljivka " + zeton + ".");
    return;
  }

  izhod[tI] << "\tmov rax, [" << glob[spre[zeton].glob] << " - "
            << spre[zeton].pol * 8 << "]" << endl;
}

void izraz(string &s, int &ind);

void vrednost(string &s, int &ind) {
  ign(s, ind);
  if (s[ind] == '-') {
    ind++;
    vrednost(s, ind);
    izhod[tI] << "\tneg rax" << endl;
  } else if (s[ind] == '+') {
    ind++;
    vrednost(s, ind);
  } else if (s[ind] == '(') {
    ind++;
    izraz(s, ind);
    ind++;
  } else if (isdigit(s[ind])) {
    stevilo(s, ind);
  } else {
    spremenljivka(s, ind);
  }

  return;
}

void zmnozi(string &s, int &ind) {
  ind++;

  izhod[tI] << "\tpush rax" << endl;
  vrednost(s, ind);

  izhod[tI] << "\tpop rbx" << endl;
  izhod[tI] << "\timul rax, rbx" << endl;
}

void deli(string &s, int &ind) {
  ind++;

  izhod[tI] << "\tpush rax" << endl;
  vrednost(s, ind);

  izhod[tI] << "\tmov rbx, rax" << endl;
  izhod[tI] << "\tpop rax" << endl;
  izhod[tI] << "\txor rdx, rdx" << endl;
  izhod[tI] << "\tidiv rbx" << endl;
}

void clen(string &s, int &ind) {
  vrednost(s, ind);
  while (ind < s.length() - 1) {
    ign(s, ind);
    if (s[ind] == '*')
      zmnozi(s, ind);
    else if (s[ind] == '/')
      deli(s, ind);
    else
      break;
  }
}

void sestej(string &s, int &ind) {
  ind++;

  izhod[tI] << "\tpush rax" << endl;
  clen(s, ind);

  izhod[tI] << "\tpop rbx" << endl;
  izhod[tI] << "\tadd rax, rbx" << endl;
}

void odstej(string &s, int &ind) {
  ind++;

  izhod[tI] << "\tpush rax" << endl;
  clen(s, ind);

  izhod[tI] << "\tpop rbx" << endl;
  izhod[tI] << "\tsub rax, rbx" << endl;
  izhod[tI] << "\tneg rax" << endl;
}

void izraz(string &s, int &ind) {
  ign(s, ind);

  if (s[ind] == '+' || s[ind] == '-')
    izhod[tI] << "\txor rax, rax" << endl;
  else
    clen(s, ind);

  while (ind < s.length() - 1) {
    ign(s, ind);
    if (s[ind] == '+')
      sestej(s, ind);
    else if (s[ind] == '-')
      odstej(s, ind);
    else
      break;
  }
}

void pisi(string &s, int &ind) {
  ign(s, ind);

  izraz(s, ind);

  izhod[tI] << "\tcall pisi" << endl;
}

void nastaviSpremenljivko(string &s, int &ind, string ime) {
  if (!spre.count(ime)) {
    definirajSpremenljivko(ime);
  }
  ign(s, ind);
  if (s[ind] != '=') {
    napaka(s, ind, "Neveljavno nastavljanje spremenljivke " + ime + ".");
    return;
  }
  ind++;
  ign(s, ind);

  izraz(s, ind);

  izhod[tI] << "\tmov [" << glob[spre[ime].glob] << " - " << spre[ime].pol * 8
            << "], rax" << endl;
}

int stVej = 0;
stack<pair<int, int>> veje; // st veje, tip veje
void zaZanka(string &s, int &ind) {
  stVej++;
  veje.push({stVej, 1});

  ign(s, ind);
  string ime = dobiZeton(s, ind);

  if (!spre.count(ime)) {
    definirajSpremenljivko(ime);
  }

  ign(s, ind);

  string zeton = dobiZeton(s, ind);

  if (zeton == "od") {
    ign(s, ind);
    izraz(s, ind);

    izhod[tI] << "\tmov [" << glob[spre[ime].glob] << " - " << spre[ime].pol * 8
              << "], rax\n"
              << endl;

    ign(s, ind);
    zeton = dobiZeton(s, ind);
  }

  if (zeton != "do") {
    napaka(s, ind, "Neveljavna zanka" + zeton);
    return;
  }
  ind++;
  ign(s, ind);

  izhod[tI] << "\tmov rax, [" << glob[spre[ime].glob] << " - "
            << spre[ime].pol * 8 << "]\n\tdec rax\n\tmov ["
            << glob[spre[ime].glob] << " - " << spre[ime].pol * 8
            << "], rax\nzanka" << stVej << ":\n\tmov rax, ["
            << glob[spre[ime].glob] << " - " << spre[ime].pol * 8
            << "]\n\tinc rax\n\tmov [" << glob[spre[ime].glob] << " - "
            << spre[ime].pol * 8 << "], rax" << endl;

  izraz(s, ind);
  izhod[tI] << "\tmov rbx, [" << glob[spre[ime].glob] << " - "
            << spre[ime].pol * 8 << "]\n\tcmp rax, rbx\n\tjl konec" << stVej
            << endl;
}

bool relacijskiZnak(char c) {
  string znaki = "<>=";
  for (char &z : znaki)
    if (z == c)
      return true;
  return false;
}

void pogoj(string &s, int &ind) {
  izraz(s, ind);
  izhod[tI] << "\tpush rax" << endl;

  ign(s, ind);
  string relacija = "";
  while (relacijskiZnak(s[ind]))
    relacija += s[ind++];

  ign(s, ind);

  izraz(s, ind);
  izhod[tI] << "\tpop rbx\n\tcmp rax, rbx\n\t";

  if (relacija == ">")
    izhod[tI] << "jge";
  else if (relacija == ">=")
    izhod[tI] << "jg";
  else if (relacija == "<")
    izhod[tI] << "jle";
  else if (relacija == "<=")
    izhod[tI] << "jl";
  else if (relacija == "=")
    izhod[tI] << "jne";
  else
    napaka(s, ind, "Neznana relacija.");

  izhod[tI] << " konec" << stVej << endl;
}

void doklerZanka(string &s, int &ind) {
  veje.push({++stVej, 1});
  izhod[tI] << "zanka" << stVej << ":" << endl;
  pogoj(s, ind);
}

void pogojniStavek(string &s, int &ind) {
  veje.push({++stVej, 0});
  pogoj(s, ind);
}

string trenFunk = "";
void definirajFunkcijo(string &s, int &ind) {
  if (tI) {
    napaka(s, ind, "Poskus definiranja funkcije znotraj funkcije");
    return;
  }
  if (veje.size()) {
    napaka(s, ind, "Poskus definiranja funkcije znotraj veje.");
    return;
  }
  stVej++;

  veje.push({-1, -1});

  ign(s, ind);

  string ime = dobiZeton(s, ind);

  if (spre.count(ime)) {
    napaka(
        s, ind,
        "Napaka pri definiranju funkcije.\nObstaja ze spremenljivka z imenom " +
            ime + ".");
  }
  definirajSpremenljivko(ime);
  trenFunk = ime;

  tI = 1;
  izhod[tI] << "funkcija" << stVej << ":" << endl;
  ign(s, ind);

  int stArgumentov = 0;
  while (ind < s.length()) {
    string zeton = dobiZeton(s, ind);

    if (spre.count(zeton)) {
      napaka(s, ind, zeton + " ze obstaja kot globalna spremenljivka.");
      return;
    }
    shraniSpremenljivko(zeton);
    stArgumentov++;
    ign(s, ind);
  }

  funk[ime] = (funkcija){stVej, stArgumentov};
  izhod[tI] << "\tpush rbp\n\tmov rbp,rsp\n\tsub rsp, " << stArgumentov * 8
            << endl;
}

void kliciFunkcijo(string &s, int &ind, string ime) {
  izhod[tI] << "\tpush 0\n\tpush 0" << endl;
  while (ind < s.length()) {
    izraz(s, ind);
    izhod[tI] << "\tpush rax" << endl;
  }
  izhod[tI] << "\tadd rsp, " << (funk[ime].stArgumentov + 2) * 8
            << "\n\tcall funkcija" << funk[ime].pol << endl;
}

void sicerStavek(string &s, int &ind) {
  if (veje.top().second) {
    napaka(s, ind, "Napačna uporaba \"sicer\" stavka.");
    return;
  }

  int naslednjaVeja = veje.top().first;
  veje.pop();

  veje.push({++stVej, 0});

  izhod[tI] << "\tjmp konec" << stVej << endl;
  izhod[tI] << "konec" << naslednjaVeja << ":" << endl;
}

void izhodIzFunkcije(string &s, int &ind) {
  if (!izhod) {
    napaka(s, ind, "Poskus izhoda iz funkcije v globalni kodi.");
    return;
  }

  izraz(s, ind);
  izhod[tI] << "\tmov [" << glob[spre[trenFunk].glob] << " - "
            << spre[trenFunk].pol * 8 << "], rax" << endl;

  izhod[tI] << "\tmov rsp, rbp\n\tpop rbp\n\tret" << endl;
}

void konecVeje() {
  if (veje.top().first == -1) {
    izhod[tI] << "\tmov rsp, rbp\n\tpop rbp\n\tret" << endl;
    for (string &s : lokSpre) {
      spre.erase(s);
    }
    tI = 0;
    veje.pop();
    return;
  }
  if (veje.top().second) {
    izhod[tI] << "\tjmp zanka" << veje.top().first << endl;
  }

  izhod[tI] << "konec" << veje.top().first << ":" << endl;
  veje.pop();
}

void vrstica(string &s) {
  int ind = 0;

  ign(s, ind);
  if (ind == s.size())
    return;
  izhod[tI] << "\n";

  string zeton = dobiZeton(s, ind);

  if (zeton == "pisi") {
    pisi(s, ind);
  } else if (zeton == "za") {
    zaZanka(s, ind);
  } else if (zeton == "dokler") {
    doklerZanka(s, ind);
  } else if (zeton == "ce") {
    pogojniStavek(s, ind);
  } else if (zeton == "sicer") {
    sicerStavek(s, ind);
  } else if (zeton == "konec") {
    konecVeje();
  } else if (zeton == "vrni") {
    izhodIzFunkcije(s, ind);
  } else if (zeton == "fun") {
    definirajFunkcijo(s, ind);
  } else if (funk.count(zeton)) {
    kliciFunkcijo(s, ind, zeton);
  } else if (zeton != "") {
    nastaviSpremenljivko(s, ind, zeton);
  } else {
    izraz(s, ind);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Uporaba: " << argv[0] << " <ime_datoteke>" << endl;
    return 1;
  }

  string imeDatoteke = argv[1];
  ifstream vhod(imeDatoteke);
  if (!vhod.is_open()) {
    std::cerr << "Napaka pri odpiranju!" << std::endl;
    return 1;
  }

  izhod[0] << "section .text\n\tglobal _start\n\n_start:\n\tmov rbp, "
              "rsp\n\tmov r15, rsp\n";

  string s;
  while (getline(vhod, s)) {
    vrstica(s);
    stVrstice++;
  }
  vhod.close();

  izhod[0] << "\n\txor rdi, rdi\n\tmov rax, 60\n\tsyscall" << endl;
  izhod[1]
      << "\npisi:\n\tpush rbp\n\tmov rbp, rsp\n\n\ttest rax,rax\n\tjz "
         "pozitivno\n\ttest rax, (1 << 63) - 1\n\tjg pozitivno\n\n\tneg "
         "rax\n\tpush rax\n\tpush '-'\n\tmov rax, 1\n\tmov rdi, 1\n\tmov rsi, "
         "rsp\n\tmov rdx, 1\n\tsyscall\n\tadd rsp, 8\n\tpop "
         "rax\n\npozitivno:\n\n\tmov rcx, 10\n\tmov rbx, 1\n\tsub rsp, "
         "1\n\tmov byte [rsp], 10\n\nstevka_loop:\n\txor rdx, rdx\n\tdiv "
         "rcx\n\tadd dl, '0'\n\tsub rsp, 1\n\tmov [rsp], dl\n\tinc rbx\n\ttest "
         "rax, rax\n\tjnz stevka_loop\n\n\tmov rsi, rsp\n\tmov rax, 1\n\tmov "
         "rdi, 1\n\tmov rdx, rbx\n\tsyscall\n\n\tmov rsp, rbp\n\tpop rbp\n\tret"
      << endl;

  ofstream izhodnaDatoteka;
  izhodnaDatoteka.open("out.asm");
  izhodnaDatoteka << izhod[0].str() << "\n\n" << izhod[1].str() << endl;

  if (veje.size()) {
    cout << "Nezaprte veje" << endl;
    return 1;
  }

  cout << "Prevedeno" << endl;

  return 0;
}
