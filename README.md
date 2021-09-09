# AngryBlocks
Projekt semestr 4, Programowanie Komputerów

# Temat

Celem programu jest stworzenie gry dla 1 gracza, którego zadaniem jest rzucanie obiektem w taki sposób, żeby strącić wszystkie wrogie obiekty. Utrudnieniem jest obecność przeszkód w postaci bloczków o różnych kształtach. Przeszkody można zniszczyć, ale występują one w 3 poziomach wytrzymałości: drewno, kamień, metal. 
Gracz dysponuje 3 rzutami, gdy po wykorzystaniu 3 rzutów nie uda się strącić wszystkich wrogich obiektów, poziom jest przegrany. W grze dostępne będą dla gracza 3 rodzaje obiektów do rzucania: zwykły, odłamkowy i bombardujący. Rzucać będzie można, przeciągając myszą w przeciwną stronę do zamierzonego przez nas kierunku ruchu. Trajektoria lotu będzie pokazywana kilkunastoma kropkami.
W grze będzie 5 poziomów do ukończenia. Zniszczenia przeszkód, zniszczenia bloczków przeciwnika i niewykorzystanie wszystkich obiektów do rzucania przyznają graczowi dodatkowe punkty. Punkty będą zapisywane dla każdej planszy z osobna i po uzyskaniu większej niż poprzednio ilości punktów, wynik będzie zastępowany.

# Analiza tematu

W programie odbywa się symulacja fizycznego świata z grawitacją, obiektami i kolizjami, które odbywają się między symulowanymi obiektami. Obliczenia odpowiedzialne za tę symulację są realizowane przez bibliotekę Box2D. Jest to biblioteka, która nie ma możliwości graficznego przedstawienia tej symulacji, dlatego konieczne jest użycie innej biblioteki za to odpowiedzialnej.
W tym programie za graficzną reprezentację odpowiedzialna jest biblioteka SFML. Problemem połączenia obu bibliotek jest różnica w używanych przez nie jednostek. 
Do wyrażenia miary odległości SFML używa pikseli a Box2D metrów. Do wyrażenia miary kąta SFML używa stopni a Box2D radianów. Połączenie obu bibliotek, żeby synchronicznie ze sobą działały, wymaga zastosowania konwersji jednostek. Funkcje odpowiedzialne za tą konwersję znajdują się w przestrzeni nazw Converter. Sam obiekt, który jest widoczny dla użytkownika (SFML) i którego fizyka jest symulowana (Box2D), jest realizowany przez klasę SimulatedObject.
Wygrana oznacza, że na mapie nie znajduje się żaden obiekt o rodzaju przeciwnik. Przegrana oznacza, że każdy z obiektów do rzucania został użyty i każdy z nich skończył się poruszać.
Rzucanie odbywa się poprzez obliczenie różnicy koordynatu początkowego obiektu do rzucania a koordynatu myszki w momencie puszczenia lewego przycisku myszy. W tym momencie następuje impulsywne nałożenie prędkości liniowej w centrum masy obiektu.  
Trajektoria lotu w momencie trzymania lewego przycisku myszy realizowana jest wzorem:

![rownanie](https://user-images.githubusercontent.com/49723341/132680418-6f8e6e7d-4a5c-4c75-b31d-3e5dd1c1dbcf.png)

Gdzie:  
p(n) - pozycja w skoku czasowym n  
p0 - pozycja początkowa (zerowy skok czasowy)  
v - początkowa prędkość na skok czasowy  
a - przyspieszenie ziemskie na skok czasu

Poziom jest tworzony, interpretując plik tekstowy lub binarny i zapisując projekt poziomu w przeznaczonej na to trójwymiarowej, statycznej tablicy znaków. Interpretacja polega na liniowym przeszukiwaniu pliku aż do jego końca i zapisie odczytanych znaków, jeżeli wczytana linia zawiera odpowiednią ilość znaków.
Przykład projektu poziomu w pliku tekstowym:

![levels](https://user-images.githubusercontent.com/49723341/132681105-945196c4-4a1f-4fff-ab11-c123fcbb6d79.png)

Gdzie:  
w - przeszkoda drewniana  
s - przeszkoda kamienna  
m - przeszkoda metalowa  
e - przeciwnik  
g - podłoże

W trakcie działania programu można zapisać zinterpretowany plik tekstowy do pliku w postaci binarnej.
Na początku działania programu wczytywane są wcześniej zdobyte punkty na każdym z poziomów, a pod koniec programu są one zapisywane. Znów można podjąć decyzje zapisu/odczytu z pliku binarnego lub tekstowego, przy pomocy parametru metody.

# Specyfikacja zewnętrzna
Przydatne skróty klawiszowe dla użytkownika:  
R - restart aktualnego poziomu  
1-3 - wybór obiektu do rzucania  
M - menu map  
Esc - wyjście z programu

Po uruchomieniu programu użytkownikowi ukazuje się menu z wszystkimi dostępnymi mapami. Mapa jest dostępna do kliknięcia, jeżeli poprzednia mapa została pokonana.

![mapmenu](https://user-images.githubusercontent.com/49723341/132681580-1e56275b-2f19-45d3-b49e-11bad5c6b4c5.png)

Niedostępne mapy są przeźroczyste i nie podświetlają się po najechaniu na nie myszką.

![mapmenu2](https://user-images.githubusercontent.com/49723341/132681597-958db9b2-bb7d-4387-a28b-1e4181a62315.png)

Po kliknięciu, na ekranie wyświetla się odpowiedni poziom z dostępnymi obiektami do rzucania dostosowanymi do trudności poziomu. Trudność poziomu zależy od ilości i rodzaju przeszkód (drewno - łatwe, kamień - średni, metal - ciężki), ilości przeciwników i ich umieszczenia.

![level](https://user-images.githubusercontent.com/49723341/132681634-873643f6-1cf6-47ed-a886-aee25e4e7e55.png)

W lewym górnym rogu ekranu znajduje się ilość punktów zdobytych w aktualnej próbie, pod nią jest maksymalna ilość punktów, jaką udało się zdobyć na tym poziomie.

![scores](https://user-images.githubusercontent.com/49723341/132681651-af1d3c5b-c61c-4a0e-8861-84a0c9951230.png)

Pod punktami znajdują się dostępne rzuty wraz ze skrótem klawiszowym do ich wyboru. Użyte obiekty są transparentne i niemożliwe do ponownego wyboru.

![throwables](https://user-images.githubusercontent.com/49723341/132681673-d48e23ba-3c39-465e-987f-94efeb51f950.png)

W prawym górnym rogu znajduje się przycisk manu map, który przenosi do tego samego menu, które wyświetla się po starcie programu. W menu map znajduje się ten sam przycisk, który przenosi z powrotem do aktualnego poziomu.

![mapbutton](https://user-images.githubusercontent.com/49723341/132681697-6cf49f42-d86d-425c-9d7a-40971d9c5a14.png)

Gdy użytkownik zdecydował się na jeden z typów obiektu do rzucania, może rozpocząć celowanie przez przytrzymanie lewego przycisku myszy.

![trajectory](https://user-images.githubusercontent.com/49723341/132681704-f6d3ff6b-d1d1-44ef-b29a-f36d3e31b77d.png)

Puszczenie przycisku powoduje wyrzut obiektu.

Do wyboru są 3 typy obiektów do rzucania:
1. Zwyczajny - brak specjalnych efektów

![normal](https://user-images.githubusercontent.com/49723341/132681744-028f39c3-0dd0-46b9-9f26-509ec9611870.png)

2. Odłamkowy - po kolizji tworzy 8 odłamków

![cluster](https://user-images.githubusercontent.com/49723341/132681753-c7a0c614-baae-44e5-a539-f2fe5b084d81.png)

3. Bombardujący - po wciśnięciu lewego przycisku myszy tworzy bombę (maksymalnie 3)

![bombarding](https://user-images.githubusercontent.com/49723341/132681770-a64ecf6d-28a2-48f9-94ec-ea4c81b54081.png)

Przyjmowanie obrażeń przez przedmioty jest zobrazowane w taki sposób:

![destr](https://user-images.githubusercontent.com/49723341/132681797-ffc831ab-c4fe-4ab3-9438-5eb429b32ce1.png)

Po przegraniu lub wygraniu wyświetlają się odpowiednie okna, które pozwalają na powtórzenie poziomu lub przejście do następnego w przypadku wygranej.

![restart](https://user-images.githubusercontent.com/49723341/132681832-f7ed3e8a-569e-43c6-a8a1-21bac4068b6b.png)
![next](https://user-images.githubusercontent.com/49723341/132681837-5a0a8f1e-e1d5-422a-9981-6b76f3bc4b18.png)

Ostatnie 2 poziomy odbywają się na księżycu o obniżonym przyspieszeniu grawitacyjnym.

![moon](https://user-images.githubusercontent.com/49723341/132681854-237725be-14ad-4c84-a40e-ec611465fc66.png)

# Specyfikacja wewnętrzna
Program został zrealizowany zgodnie z paradygmatem strukturalnym. W programie rozdzielono interfejs (komunikację z użytkownikiem) od logiki aplikacji (symulacji fizyki).

# Klasy
1. Screen
Odpowiada za stworzenie okna, w którym wyświetlany będzie program, za komunikację z użytkownikiem, operacje na plikach i koordynacje stanów gry (poziom, wygrana przegrana, menu map).
Zawiera w sobie obiekty klasy Button do stworzenia przycisków i wskaźnik na obiekt klasy Level.

2. Button
Odpowiada za stworzenie interaktywnego przycisku, który wykrywa pozycje kursora myszy na ekranie i potrafi poinformować o kliknięciu.
Znajduje się w klasie Screen.

3. Level
Odpowiada za tworzenie i przechowywanie w sobie wszystkich obiektów poziomu, których fizyka jest symulowana, za interakcje między tymi elementami, wykrywanie kolizji między nimi i niszczenie ich.
Zawiera w sobie obiekty klasy SimulatedObject w tym: Obstacle, Ground, Enemy, Throwable i ich rodzaje. Zawiera w sobie pojedynczą instancję klasy ContactListener. Występuje w klasie Screen.

4. ContactListener
Odpowiada za wykrywanie kolizji i identyfikacji, które ciała w nich uczestniczą. Klasa ta zwraca informacje o kolizji tylko wtedy gdy 2 ciała się stykają, dzięki temu nie ma konieczności iterowania przez każdy obiekt i sprawdzania, czy jego granice stykają się z granicami każdego innego obiektu na poziomie. Miałoby to złożoność obliczeniową O(n2).
Dziedziczy z b2ContactListener - klasy biblioteki Box2D. Jej instancja jest obecna w klasie Level.

5. SimulatedObject
Odpowiada za połączenie dwóch bibliotek zewnętrznych: SFML (grafika) i Box2D (fizyka). Wykonuje odpowiednie konwersje jednostek (stopnie-radiany i piksele-metry) z użyciem przestrzeni nazw Converter.
Konwersja jest konieczna, żeby grafiki obiektów pokazywały się w tym miejscu na ekranie, na który wskazują obliczenia Box2D.
Jest to klasa abstrakcyjna z czysto wirtualną metodą createObject, z której dziedziczą wszystkie symulowane obiekty występujące na poziomie. Jej klasami potomnymi są Obstacle, Ground, Enemy i Throwable. Kolekcja wszystkich symulowanych obiektów znajduje się w klasie Level w jej wektorze wskaźników na SimulatedObject.

6. Obstacle
Reprezentuje przeszkode - dynamiczny (możliwy do poruszenia) obiekt, który może ulec zniszczeniu. Występuje w 3 typach: drewno, kamień, metal. Typ przeszkody wpływa na maksymalne HP, teksturę i gęstość obiektu.
Dziedziczy z klasy SimulatedObject.

7. Ground
Reprezentuje podłoże - statyczny (niemożliwy do poruszenia) obiekt, który nie może ulec zniszczeniu. Występuje w 2 typach: zwykły i księżycowy. Typ podłoża zależy od rodzaju poziomu.
Dziedziczy z klasy SimulatedObject.
Zawiera dwa konstruktory: jeden oparty o teksture - w którym wybierany jest typ podłoża, drugi oparty o kolor (tworzy jednolity blok)

8. Enemy
Reprezentuje przeciwnika - dynamiczny (możliwy do poruszenia) obiekt, który może ulec zniszczeniu. Wszystkie obiekty tego typu muszą zostać zniszczone, żeby obiekt typu Level mógł podjąć decyzje o zwycięstwie.
Dziedziczy z klasy SimulatedObject.

9. Throwable
Do bazy, jaką stanowi klasa SimulatedObject klasa Throwable wprowadza swoje metody i pola, które umożliwią celowanie i wystrzelenie obiektu oraz sprawdzanie, z czy obiekt wciąż się porusza. Zawiera 3 stany:
- Oczekujący (Idle) - wyświetlana jest grafika obiektu i oczekuje na kliknięcie, aby wyświetlić trajektorię
- Celujący (Aiming) - wyświetlana jest trajektoria lotu
- Użyty (Used) - obiekt został użyty i jego fizyka jest symulowana
Stan obiektu (użyty) i stwierdzenie nieporuszania się umożliwia poziomowi podjęcie decyzji o przegranej.
Dziedziczy z klasy SimulatedObject.

10. DefaultThrowable
Nie zawiera żadnych specjalnych efektów. Dziedziczy z klasy Throwable.
W konstruktorze obiektu klasy DefaultThrowable ustalony zostaje typ obiektu, jego lokacja, tekstura i rozmiar.

11. Cluster
Tworzy 8 odłamków po kolizji obiektu. Stworzone odłamki są wstrzeliwane z prędkością liniową w 4 różnych kierunkach.
Dziedziczy z klasy Throwable.
W konstruktorze obiektu klasy Cluster ustalony zostaje typ obiektu, jego lokacja, tekstura, rozmiar oraz to czy obiekt nie jest odłamkiem. Jest to konieczne, ponieważ odłamki nie mogą tworzyć kolejnych odłamków.

12. Bombarding
Tworzy maksymalnie 3 bomby od obiektu typu Bombarding. Wystrzelenie bomby jest kontrolowane przez użytkownika lewym przyciskiem myszy w trakcie gdy obiekt typu Bombarding został użyty, ale jeszcze nie skończył się poruszać. Bomba jest wystrzeliwana z prędkością liniową skierowaną w dół.
Dziedziczy z klasy Throwable.

# Szczegółowy opis klas i metod
Szczegółowy opis klas i metod zawarty w pliku "Dokumentacja - AngryBlocks.pdf".

# Diagram hierarchii klas
Dziedziczenie oznaczone jest białą strzałką a powiązania zieloną.

![classes](https://user-images.githubusercontent.com/49723341/132682397-1be03e40-ee66-4148-9421-2397a4beb087.png)

# Istotne struktury danych i algorytmy
Istotnymi strukturami danych programu są:

1. Statyczna tablica trójwymiarowa [5x22x41] znaków klasy Screen - wczytana z pliku tekstowego lub binarnego, określa design poziomów - pierwszy wymiar oznacza numer poziomu, drugi wysokość a trzeci szerokość poziomu. Wczytanie designu z pliku i zapisanie w tej tablicy umożliwia tworzenie poziomu bez konieczności ponownego korzystania z dysku - plik jest otwierany tylko na początku programu.
2. Wektor biblioteki STL klasy Level - wektor będący tablicą dynamiczną nadaje się na kontener zmiennej ilości tworzonych i usuwanych obiektów na poziomie. Usuwanie obiektu z wektora odbywa się przy użyciu iteratora. Sąsiednie umieszczenie komórek pamięci przyspiesza liniowe przeszukiwanie tablicy, ponieważ usprawnia to wykorzystanie pamięci cache.

Istotne algorytmy:

Liniowe przeszukiwanie wektora objects - musi odbyć się przy wykonywaniu wielu operacji:
zaktualizowaniu pozycji poruszających się obiektów
zniszczeniu obiektów, które mają HP mniejsze lub równe 0
wykrycia obecności lub braku obiektu typu Enemy
	Algorytm ma złożoność obliczeniową O(n).

# Wykorzystane techniki obiektowe
W projekcie wykorzystane zostało dziedziczenie z klasy abstrakcyjnej SimulatedObject z czysto wirtualną funkcją createObject umożliwiającą wykorzystanie polimorfizmu.
W przestrzeni nazw Converter zostało wykorzystane programowanie generyczne (wzorce), aby funkcje mogły przyjmować różne typy danych. 
W używanych przeze mnie klasach użyłem enkapsulacji szczegółów implementacji np. łączenia biblioteki graficznej z fizyczną w klasie SimulatedObject.
Do przechowywania i przeszukiwania symulowanych obiektów poziomu wykorzystane są kontenery i iteratory STL.
W programie został użyty mechanizm wyjątków.

# Ogólny schemat działania programu
W funkcji głównej tworzona jest instancja main_screen klasy Screen. Konstruktor tego obiektu wczytuje tekstury, czcionkę, napisy i wyskakujące okna. Następnie ładowane są punkty z pliku binarnego metodą loadScore i design poziomów z pliku binarnego lub tekstowego (zależnie który zostanie znaleziony) z użyciem metody loadLevelsLayout. Jeżeli nie uda się wczytać punktów z pliku zostanie wypisany komunikat "Unable to open text file" lub "Unable to open binary file".
Następnie tworzone jest okno programu w metodzie createWindow. Znajduje się tam pętla, która trwa, dopóki okno jest otwarte. Zawiera ona w sobie instrukcje warunkowe sprawdzające aktualny stan gry - menu map lub poziom.
Gdy stanem gry jest menu map, renderowane jest okno, w którym są przyciski do załadowania poziomu. Po kliknięciu przycisku odpowiadającemu danemu poziomowi tworzony jest obiekt klasy Level i wczytywany jego design i highscore na podstawie numeru klikniętego przycisku a wskaźnik na wczytany poziom znajduje się w zmiennej active_level.
Gdy stanem gry jest poziom, renderowane są wszystkie obiekty poziomu oraz napisy z punktacją, sprawdzane są warunki spełnienia wygranej lub przegranej, aby móc przejść do stanu gry wygranej lub przegranej. W stanie poziom odbywają się również wszystkie operacje konieczne do przeprowadzenia gry takie jak - aktualizowanie pozycji obiektów, niszczenie ich, tworzenie obiektów do rzucania i przyznawanie punktów.
W tej pętli znajduje się również kolejna pętla sprawdzająca, czy wczytywany jest jakiś input/event od użytkownika taki jak np. kliknięcie lewego przycisku myszy, czy klawisza Esc, po którym zamknięte zostaje okno programu, zapisanie wyników metodą saveScore i zakończenie pracy programu.

# Testowanie i uruchamianie
Program został przetestowany pod kątek ilości obiektów, które może stworzyć i renderować przy rozsądnych klatkach na sekundę - program działa przy grywalnych klatkach na sekundę do około 500 obiektów na poziomie. Przy większych ilościach stworzonych obiektów program działa powoli aż do momentu zniszczenia się obiektów przez kolizję do ilości około 500 obiektów. Uruchomienie poziomu z 10000 obiektami trwa 16 sekund. Przy 40000 obiektach uzyskuje się błąd:

![testing1](https://user-images.githubusercontent.com/49723341/132682511-77c95d5a-a2f5-4ace-a913-e32031c64d0f.png)

Przy 50000 obiektach występuje inny błąd:

![testing2](https://user-images.githubusercontent.com/49723341/132682592-ec9eaa89-c90d-4bc0-8cc6-d891efffd46c.png)

Poziom z największą ilością obiektów, jaką udało się uruchomić to 30000 (są to drewniane pudełka wielkości 5 × 5, gdzie standardowe są wielkości 35 × 35).

![maxblocks](https://user-images.githubusercontent.com/49723341/132682641-a65b6dc1-9cbb-4e0f-9e7b-b5b6170c7b32.png)

Stworzenie poziomu z 30000 obiektów trwało 56 sekund.
Odpalenie programu po usunięciu folderu „Levels” gdzie znajdują się punkty i design poziomów skutkuje wyzerowaniem uzyskanych punktów dla każdego z poziomów, a każdy poziom tworzy się pusty. Jeżeli przy tworzeniu poziomów w pliku tekstowym popełni się błąd otrzymuje się komunikat "Line is not 41 chars!" interpretacja pliku się kończy i od tego momentu design jest określany przez plik binarny.
Program został sprawdzony pod kątem wycieków pamięci z użyciem biblioteki CRT.

Program uruchamia się plikiem wykonywalnym „AngryBlocks.exe”.

![running](https://user-images.githubusercontent.com/49723341/132682664-024b8ba1-561a-42fc-a8ac-f233dc8a6f76.png)

# Źródła
https://www.sfml-dev.org/tutorials/2.5/

https://box2d.org/documentation/

https://www.iforce2d.net/b2dtut/collision-callbacks

http://www.iforce2d.net/b2dtut/projected-trajectory

Tekstury:

https://opengameart.org/content/platformer-art-complete-pack-often-updated

https://opengameart.org/content/bevouliin-free-mountain-game-background

https://opengameart.org/content/gui-buttons-vol1

https://opengameart.org/content/galaxy-skybox
