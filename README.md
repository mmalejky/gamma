Zadanie gamma, część 1
Tegoroczne duże zadanie polega na zaimplementowaniu gry gamma. Grę rozgrywa się na prostokątnej planszy, która składa się z jednakowych kwadratowych pól. Pola sąsiadujące na planszy to takie, które stykają się bokami. Pól stykających się tylko rogami nie uważamy za sąsiadujące. Pola tworzą obszar, jeśli z każdego z nich można dojść do innego, przechodząc jedynie przez pola sąsiadujące. Pojedyncze pole też jest obszarem. Grać może jedna lub więcej osób. Na początku gry plansza jest pusta. Gracze kolejno zajmują po jednym polu, stawiając na nim swój pionek. Gracz może zająć dowolne niezajęte pole, przestrzegając jedynie zasady, że zbiór pól zajętych przez tego samego gracza nie może w żadnej fazie gry składać się z więcej niż, będącej parametrem gry, maksymalnej liczby obszarów. Każdy gracz jeden raz w całej rozgrywce może wykonać złoty ruch, który polega na zabraniu z planszy pionka innego gracza i postawieniu w jego miejsce swojego pionka, ale ten ruch nadal nie może naruszać zasady maksymalnej liczby zajętych obszarów przez żadnego z graczy. Gracz, który nie jest w stanie wykonać ruchu zgodnego z powyższą zasadą, wypada z gry, ale może wrócić do gry po jakimś złotym ruchu innego gracza. Gra kończy się, gdy już żaden gracz nie może wykonać ruchu. Wygrywa gracz, który zajmie największą liczbę pól.

Jako pierwszą część zadania należy zaimplementować moduł „silnika” gry. Opis interfejsu modułu znajduje się w pliku src/gamma.h w formacie komentarzy dla programu doxygen. Przykład użycia znajduje się w pliku src/gamma_test.c.

Kolejne części zadania będą polegały na rozbudowywaniu tego silnika i dodaniu do niego interfejsu (tekstowego).

Dostarczamy
W repozytorium https://git.mimuw.edu.pl/IPP-login.git (gdzie login to identyfikator używany do logowania się w laboratorium komputerowym) znajduje się szablon implementacji rozwiązania tego zadania. Znajdują się tam następujące pliki:

src/gamma.h – deklaracja interfejsu modułu wraz z jego dokumentacją w formacie doxygen,
src/gamma_test.c – przykład użycia modułu,
CMakeLists.txt – plik konfiguracyjny programu cmake,
Doxyfile.in – plik konfiguracyjny programu doxygen,
MainPage.dox – strona główna dokumentacji w formacie doxygen.
Zastrzegamy sobie możliwość nanoszenia poprawek do tego szablonu. Będziemy je umieszczać w gałęzi template/part1. Lista poprawek:

poprawna wartość parametru areas w funkcji gamma_new jest liczbą dodatnią.
Wymagamy
Jako rozwiązanie części 1 zadania wymagamy:

stworzenia pliku src/gamma.c z implementacją modułu,
uzupełnienia dokumentacji w formacie doxygen tak, aby była przydatna dla programistów rozwijających moduł.
Powinna być możliwość skompilowania rozwiązania w dwóch wersjach: release i debug. Wersję release kompiluje się za pomocą sekwencji poleceń:

mkdir release
cd release
cmake ..
make
make doc
Wersję debug kompiluje się za pomocą sekwencji poleceń:

mkdir debug
cd debug
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
make doc
W wyniku kompilacji odpowiednio w katalogu release lub debug powinien powstać plik wykonywalny gamma oraz dokumentacja. W poleceniu cmake powinno być również możliwe jawne określenie wariantu release budowania pliku wynikowego:

cmake -D CMAKE_BUILD_TYPE=Release ..
Zawartość dostarczonych przez nas plików można modyfikować, o ile nie zmienia to interfejsu modułu i zachowuje wymagania podane w treści zadania, przy czym nie wolno usuwać opcji kompilacji: -std=c11 -Wall -Wextra. Zmiany mogą dotyczyć np. stylu, dokumentacji, deklaracji typedef, włączania plików nagłówkowych, implementacji funkcji jako static inline. Inne pliki źródłowe, będące częścią rozwiązania, należy umieścić w katalogu src. Funkcja main programu musi znajdować się w pliku src/gamma_test.c, ale zawartość tego pliku nie będzie oceniana w tej części zadania.

Zadanie gamma, część 2
Jako drugą część dużego zadania należy zaimplementować program, który, korzystając z modułu silnika zaimplementowanego w części pierwszej, umożliwia przeprowadzanie rozgrywki. Interfejs programu ma być tekstowy. Program ma czytać dane ze standardowego wejścia, wyniki wypisywać na standardowe wyjście, a informacje o błędach na standardowe wyjście diagnostyczne. Program działa w dwóch trybach.

Tryb wsadowy (ang. batch mode)
W trybie wsadowym program oczekuje poleceń, każde w osobnym wierszu. Rodzaj polecenia jest determinowany pierwszym znakiem wiersza. Jeśli polecenie wymaga podania parametru lub parametrów, to po tym znaku występuje odstęp składający się z co najmniej jednego białego znaku, a po tym odstępie pojawiają się kolejne parametry. Parametry polecenia są liczbami dziesiętnymi. Pomiędzy parametrami występuje odstęp składający się z co najmniej jednego białego znaku. Na końcu wiersza może wystąpić dowolna liczba białych znaków.

Na początku działania program oczekuje jednego z dwóch poleceń:

B width height players areas
I width height players areas
Polecenia te służą do utworzenia nowej gry za pomocą wywołania funkcji gamma_new. Poprawne wykonanie polecenia B jest kwitowane wypisaniem na standardowe wyjście komunikatu

OK line

gdzie line jest numerem wiersza, w którym pojawiło się to polecenie. Po czym program akceptuje polecenia:

m player x y – wywołuje funkcję gamma_move,
g player x y – wywołuje funkcję gamma_golden_move,
b player – wywołuje funkcję gamma_busy_fields,
f player – wywołuje funkcję gamma_free_fields,
q player – wywołuje funkcję gamma_golden_possible,
p – wywołuje funkcję gamma_board.
Program wypisuje wynik każdej z tych funkcji na standardowe wyjście. Wartość false wypisuje jako 0, a wartość true jako 1.

Program nie interpretuje pustych wierszy (składających się tylko ze znaku przejścia do nowego wiersza) oraz wierszy zaczynających się znakiem #.

Program kwituje każdy błędny wiersz (niepasujący do powyższego opisu) wypisaniem na standardowe wyjście diagnostyczne komunikatu

ERROR line

gdzie line jest numerem błędnego wiersza.

Wiersze na wejściu liczone są od jedynki. Liczone są wszystkie wiersze, łącznie z tymi, które nie są interpretowane.

W trybie wsadowym program kończy działanie, gdy skończą się dane na wejściu.

Dalsze szczegóły działania programu w trybie wsadowym można wydedukować, analizując załączone przykłady użycia.

Poprawne wykonanie polecenia I powoduje przejście do trybu interaktywnego.

Tryb interaktywny (ang. interactive mode)
W trybie interaktywnym program wyświetla planszę, a pod planszą wiersz zachęcający gracza do wykonania ruchu. Program prosi o wykonanie ruchu kolejnych graczy, przy czym pomija graczy, dla których funkcja gamma_free_fields zwróciła 0 i funkcja gamma_golden_possible zwróciła false. Ruch wykonuje się, przesuwając kursor na wybrane pole za pomocą klawiszy ze strzałkami, a następnie wciskając klawisz spacja, aby wykonać zwykły ruch, lub klawisz G, aby wykonać złoty ruch. Gracz może zrezygnować z ruchu, wciskając klawisz C. Wciśnięcie klawiszy literowych powinno być rozpoznawane niezależnie od wciśnięcie klawisza Shift i stanu CapsLock.

Gra kończy się, gdy już żaden gracz nie może wykonać ruchu lub po jednokrotnym wciśnięciu kombinacji klawiszy Ctrl-D. Wtedy program wypisuje ostateczną planszę gry wraz z podsumowaniem, ile pól zajął każdy z graczy.

W trybie interaktywnym program powinien uniemożliwiać użytkownikom wykonywanie błędnych akcji.

Dalsze szczegóły działania programu w trybie interaktywnym można wydedukować, oglądając załączone animacje.

Kończenie programu
Zasadniczo program powinien zakończyć się kodem 0, chyba że wystąpił jakiś krytyczny błąd – wtedy program powinien zakończyć się kodem 1. Przed zakończeniem działania program powinien jawnie zwolnić całą zaalokowaną pamięć, w szczególności powinien wywołać funkcję gamma_delete.

Dostarczamy
Nie dostarczamy żadnego kodu źródłowego. Rozwiązanie drugiej części zadania powinno korzystać z własnego, ewentualnie samodzielnie zmodyfikowanego, rozwiązania części pierwszej.

Wymagamy
Jako rozwiązanie części 2 zadania wymagamy:

umieszczenia kodu źródłowego implementacji w katalogu src,
uzupełnienia dokumentacji w formacie doxygen tak, aby była przydatna dla programistów rozwijających program,
dostosowania pliku konfiguracyjnego dla programu cmake.
Gotowe rozwiązanie powinno się kompilować w dwóch wersjach: release i debug, jak to opisano w pierwszej części zadania.

UWAGA 1: funkcja main programu musi znajdować się w pliku src/gamma_main.c.

UWAGA 2: w wyniku kompilacji powinien powstać plik wykonywalny gamma.

Oddawanie rozwiązania
Rozwiązanie należy oddawać, podobnie jak części 1, przez repozytorium git. W repozytorium mają się znaleźć wszystkie pliki niezbędne do zbudowania pliku wykonywalnego i dokumentacji. W repozytorium nie wolno umieszczać plików binarnych ani tymczasowych. W Moodlu jako rozwiązanie należy umieścić tekst zawierający identyfikator commitu finalnej wersji rozwiązania, na przykład:

518507a7e9ea50e099b33cb6ca3d3141bc1d6638
Rozwiązanie należy zatwierdzić (git commit) i wysłać do repozytorium (git push) przed terminem podanym w Moodlu.

Wskazówki
Funkcje scanf, atoi itp. nie sprawdzają dokładnie poprawności wczytywanych wartości liczbowych. Lepiej zachowują się funkcje strtoul, strtoull lub tp.

Obsługę terminala Linuksowego w trybie interaktywnym można zrealizować za pomocą ANSI escape codes. Można też skorzystać z jakiejś biblioteki, o ile jest zainstalowana w laboratorium komputerowym.

Zadanie gamma, część 3
Celem trzeciej części zadania jest dokończenie projektu gry gamma. Oczekujemy poprawienia ewentualnych błędów z poprzednich części zadania oraz zmodyfikowania programu. Obowiązują ustalenia z treści poprzednich części zadania i z forum dyskusyjnego dla studentów.

Modyfikacja silnika gry
Należy poprawić działanie funkcji gamma_golden_possible, aby dawała wynik true wtedy i tylko wtedy, gdy gracz o numerze player może wykonać złoty ruch. Należy też stosownie zmodyfikować dokumentację tej funkcji.

Modyfikacja trybu interaktywnego
Należy uatrakcyjnić wygląd planszy w trybie interaktywnym. Przykładowe pomysły:

dodanie kolorów, na przykład w celu wyróżnienia pól gracza, który ma wykonać ruch albo w celu odróżniania pól, gdy gracze mają wielocyfrowe numery i na planszy nie ma żadnego separatora między polami,
ulepszenie komunikatów pojawiających się pod planszą w trakcie gry i po jej zakończeniu,
jeśli pole gry zajmuje więcej niż jeden znak i na tym polu stoi kursor, to całe pole wyświetlane jest w negatywie,
sprawdzanie rozmiaru okna terminala i niedopuszczanie do gry (ze stosownym komunikatem), gdy jest za małe.
Modyfikacja skryptu budującego
Należy dodać możliwość utworzenia pliku wykonywalnego z testami silnika gry. Czyli na przykład po wykonaniu:

mkdir release
cd release
cmake ..
polecenie make tworzy plik wykonywalny gamma całej gry,
polecenie make test tworzy plik wykonywalny gamma_test z testami silnika gry,
polecenie make doc tworzy dokumentację w formacie doxygen.
Jak poprzednio funkcja main gry ma się znajdować w pliku src/gamma_main.c. Funkcja main uruchamiająca testy silnika gry ma się znajdować w pliku src/gamma_test.c – może to być plik, który został udostępniony w szablonie do części 1 zadania, lub plik z oficjalnymi testami do części 1 zadania. Można do tego pliku dodać własne testy.

Wskazówka: W pliku CMakeList.txt można dodać polecenie

# Wskazujemy plik wykonywalny dla testów silnika.
add_executable(test EXCLUDE_FROM_ALL ${TEST_SOURCE_FILES})
set_target_properties(test PROPERTIES OUTPUT_NAME gamma_test)
definiując uprzednio zmienną TEST_SOURCE_FILES.

Dostarczamy
Rozwiązanie części 3 zadania powinno korzystać z własnego rozwiązania poprzednich jego części.

Wymagamy
Jako rozwiązanie części 3 zadania wymagamy:

zachowania lub poprawienia struktury plików z poprzednich części,
zachowania lub poprawienia plików źródłowych z poprzednich części rozwiązania,
uzupełnienia pliku konfiguracyjnego dla programu cmake,
uzupełnienia dokumentacji w formacie doxygen tak, aby była przydatna dla programistów rozwijających program.
Gotowe rozwiązanie powinno się kompilować w dwóch wersjach: release i debug, jak to opisano w pierwszej części zadania.

Uwaga: Niezmiernie istotne jest, aby przestrzegać opisanej specyfikacji nazw plików.
