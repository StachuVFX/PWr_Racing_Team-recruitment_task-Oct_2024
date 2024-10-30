	Opis mojego rozwiązania:

Wszystko jest jak w poleceniu (mam nadzieję lol):
1. Pobieram nazwę pliku z punktami
2. Czytam plik
3. Tworzę vector (listę) punktów (struct Point)
4. Wrzucam vector do algorytmu na otaczające punkty
5. Wypisuję w konsoli wejściowe i wyjściowe punkty

Zastosowane rozwiązania:
- Własny struct dla punktów
- Własna funkcja do rozdzielania stringów
- Algorytm Grahama na otoczkę wypukłą (napisany przez ChatGPT bo ja takiej matmy jeszcze nie ogarniam, a nie mam czasu żeby wymyślić własny)
- Algorytm pomocniczy do obliczenia iloczynu wektorowego (też ChatGPT)
- Algorytm porównujący do sortowania względem kąta biegunowego (też ChatGPT)
- Jeśli zdążę, to dorzucę własny algorytm na otaczające punkty, ale wątpię żebym się z tym wyrobił (pomysł jest taki, żeby policzyć wzory na linie między każdą parą punktów i sprawdzić, które z nich nie mają żadnych punktów po jednej ze stron, punkty definiujące te linie są punktami otaczającymi, zdaję sobie jednak sprawę, że to rozwiązanie ma wykładniczy przyrost czasu do ilości punktów)
