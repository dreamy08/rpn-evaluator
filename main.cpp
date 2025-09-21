#include <fstream>  //std::ifstream, std::ofstream чтение, запись файлов
#include <iostream> // std::cin, std::cout, std::cerr, ios::sync_with_stdio
#include <sstream>  // std::istringstream токенизациz строки по пробелам
#include <stack>    // std::stack<long long> стек операндов для rpn
#include <string>   //std::string, std::stoll, substr/find_* для trim

using namespace std;

/* Проверяет, что строка эьо корректное целое число в десятичной записи
 и возвращает значение в файлик out.
 Возвращает true/false — удалось ли распарсить строку целиком (строка без косяков).
 */

static bool parse_int(const string &s, long long &out)
{
  if (s.empty())
    return false;
  size_t i = 0;
  if (s[0] == '+' || s[0] == '-')
    i = 1;
  if (i == s.size())
    return false;
  for (; i < s.size(); ++i)
    if (!isdigit((unsigned char)s[i]))
      return false;
  try
  {
    size_t pos = 0;
    out = stoll(s, &pos, 10);
    return pos == s.size();
  }
  catch (...)
  {
    return false;
  }
}
// Обрезает пробелы по краям (space, tab, CR). \n не попадает, т.к getline его убирает.
static string trim(const string &s)
{
  auto l = s.find_first_not_of(" \t\r");
  if (l == string::npos)
    return "";
  auto r = s.find_last_not_of(" \t\r");
  return s.substr(l, r - l + 1);
}

/* Вычисляет значение одного rpn выражения.
 Если все ок, то кладёт результат в `result` и возвращает true.
 При любой ошибке возвращает false -> ничего не кладет */

static bool eval_rpn(const string &line, long long &result)
{

  istringstream iss(line);
  string tok;
  stack<long long> st; // стек операндов

  while (iss >> tok)
  {
    long long x;

    if (parse_int(tok, x))
    {
      st.push(x); // число -> в стек
      continue;
    }

    // удинственный поддерживаемый класс токенов помимо числа это бинарные операторы
    if (tok.size() == 1 && string("+-*/").find(tok[0]) != string::npos)
    {
      if (st.size() < 2)
      {
        return false; // недостаточно операндов
      }
      // первй снятый операнд - b (правый), второй - a (левый)
      long long b = st.top();
      st.pop();
      long long a = st.top();
      st.pop();
      switch (tok[0])
      {
      case '+':
        st.push(a + b);
        break;
      case '-':
        st.push(a - b);
        break;
      case '*':
        st.push(a * b);
        break;
      case '/':
        if (b == 0)
          return false; // деление на ноль -> ошибка строки
        st.push(a / b); // целочис. деление
        break;
      }
    }
    else
    {
      return false; // буква, оператор ьез проьела -> неивестный тип токенов -> false
    }
  }

  if (st.size() != 1) // поведение при правильном выражении rpn оставляет ровно одно значение в стеке
  {
    return false;
  }

  result = st.top();
  return true;
}

int main(int argc, char *argv[])
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // Пути по умолчанию можно переопределить позиционными аргументамn
  string inPath = (argc >= 2 ? argv[1] : "input.txt");
  string outPath = (argc >= 3 ? argv[2] : "output.txt");

  ifstream fin(inPath);
  if (!fin)
  {
    cerr << "Cannot open input file: " << inPath << "\n";
    return 1;
  }

  ofstream fout(outPath);
  if (!fout)
  {
    cerr << "Cannot open output file: " << outPath << "\n";
    return 1;
  }

  string line;
  while (getline(fin, line))
  {
    const string expr = trim(line); // подчистим строку убрав пробелы по краям, но не трогая внутренние пробелы между токенами
    if (expr.empty())
    {
      continue; // пустые строки скипаем (не ошбка)
    }

    long long val;
    if (eval_rpn(expr, val))
    {
      fout << expr << " = " << val << '\n';
    }
    else
    {
      fout << expr << " = ERROR\n"; // любая ошибка конкретной строки должна привести к маркеру ERROr в выводе
    }
  }
  return 0;
}
