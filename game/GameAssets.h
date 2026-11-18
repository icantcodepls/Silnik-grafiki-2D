#ifndef HOTLINE_GAMEASSETS_H
#define HOTLINE_GAMEASSETS_H
#include "engine/BitmapHandler.h"

/**
 * @file GameAssets.h
 * @brief Tekstury dema gry wczytane do BitmapHandler.
 */

/**
 * @brief Tekstury gry wczytane do BitmapHandler silnika (katalog assets, pliki PNG).
 *
 *  - `*_legs.png`  - 4 klatki 32x32 w rzędzie: cykl chodu (nogi),
 *  - `*_torso.png` - 2 klatki 32x32 w rzędzie: 0 = spokój, 1 = strzał (błysk z lufy).
 *
 * Postacie są narysowane zwrócone w PRAWO (kąt 0).
 *
 * Wczytywanie odbywa się w konstruktorze, więc obiekty gry składane z tych tekstur
 * można tworzyć dopiero po GameAssets. Gdy któregoś pliku brakuje, błąd trafia do
 * errors.log, a w jego miejsce wstawiana jest wyraźna, różowa tekstura zastępcza -
 * gra działa dalej (widać od razu, czego brakuje).
 */
struct GameAssets {
    BitmapHandler handler;                       ///< Magazyn bitmap z teksturami.
    BitmapId playerLegs = INVALID_BITMAP;        ///< Nogi gracza.
    BitmapId playerTorso = INVALID_BITMAP;       ///< Tułów gracza.
    BitmapId enemyLegs = INVALID_BITMAP;         ///< Nogi przeciwnika.
    BitmapId enemyTorso = INVALID_BITMAP;        ///< Tułów przeciwnika.
    bool allLoaded = false;                      ///< false, jeśli użyto choć jednej tekstury zastępczej.

    /// @brief Znajduje katalog assets i wczytuje wszystkie tekstury.
    GameAssets();
    GameAssets(const GameAssets&) = delete;
    GameAssets& operator=(const GameAssets&) = delete;
};

#endif // HOTLINE_GAMEASSETS_H
