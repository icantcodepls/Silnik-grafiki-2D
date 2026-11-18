#ifndef GRAFIKA2D_BITMAPHANDLER_H
#define GRAFIKA2D_BITMAPHANDLER_H
#include <unordered_map>

#include "SFML/Graphics/Image.hpp"

using namespace std;
using namespace sf;

/**
 * @file BitmapHandler.h
 * @brief Magazyn bitmap (obrazów w pamięci) adresowanych identyfikatorami oraz operacje na nich.
 */

/// @brief Identyfikator bitmapy w BitmapHandler.
using BitmapId = int;
/// @brief Wartość oznaczająca brak bitmapy / błąd.
constexpr BitmapId INVALID_BITMAP = -1;

/**
 * @brief Przechowuje bitmapy (sf::Image) i pozwala nimi zarządzać przez identyfikatory.
 *
 * Bitmapy tworzy się przez create(), createFromImage() lub load(); zwrócony BitmapId
 * służy potem do dostępu, kopiowania, przycinania itp. Klasa jest właścicielem obrazów
 * i nie można jej kopiować (obiekty graficzne trzymają do niej wskaźnik).
 * Statyczne blit() i blitTransformed() rysują jeden obraz na drugim.
 */
class BitmapHandler {
    unordered_map<BitmapId, unique_ptr<Image>> bitmaps;   ///< Zapisane obrazy.
    BitmapId nextId = 1;                                  ///< Następny wolny identyfikator.
    /// @brief Zapisuje obraz i zwraca jego nowy identyfikator.
    BitmapId store(unique_ptr<Image> image);
public:
    BitmapHandler() = default;
    BitmapHandler(const BitmapHandler&) = delete;
    BitmapHandler& operator=(const BitmapHandler&) = delete;
    /**
     * @brief Tworzy nową bitmapę wypełnioną kolorem.
     * @param w Szerokość (px).
     * @param h Wysokość (px).
     * @param fill Kolor wypełnienia.
     * @return Identyfikator nowej bitmapy lub INVALID_BITMAP przy błędzie.
     */
    BitmapId create(unsigned int w, unsigned int h, Color fill = Color::Transparent);
    /// @brief Zapisuje kopię podanego obrazu jako nową bitmapę. @param image Obraz źródłowy. @return Identyfikator.
    BitmapId createFromImage(const Image& image);
    /// @brief Wczytuje bitmapę z pliku (np. PNG). @param filename Ścieżka. @return Identyfikator lub INVALID_BITMAP.
    BitmapId load(const string& filename);
    /// @brief Zapisuje bitmapę do pliku. @param id Bitmapa. @param filename Ścieżka. @return true, gdy się udało.
    bool save(BitmapId id, const string& filename);
    /// @brief Usuwa bitmapę. @param id Bitmapa. @return true, gdy istniała.
    bool destroy(BitmapId id);
    /// @brief Usuwa wszystkie bitmapy.
    void destroyAll();
    /// @brief Sprawdza, czy bitmapa o danym identyfikatorze istnieje. @param id Bitmapa.
    bool exists(BitmapId id);
    /// @brief Zwraca liczbę przechowywanych bitmap.
    size_t count() { return bitmaps.size(); }
    /// @brief Zwraca wskaźnik na obraz lub nullptr, gdy bitmapy nie ma. @param id Bitmapa.
    Image* get(BitmapId id);
    /// @brief Zwraca rozmiar bitmapy (0,0 gdy nie istnieje). @param id Bitmapa.
    Vector2u getSize(BitmapId id);
    /// @brief Tworzy kopię bitmapy. @param id Bitmapa źródłowa. @return Identyfikator kopii.
    BitmapId copy(BitmapId id);
    /// @brief Tworzy nową bitmapę z fragmentu istniejącej. @param id Bitmapa źródłowa. @param rect Fragment (przycinany do rozmiaru).
    BitmapId crop(BitmapId id, const IntRect& rect);
    /// @brief Wypełnia całą bitmapę kolorem. @param id Bitmapa. @param fill Kolor.
    void fill(BitmapId id, Color fill);
    /// @brief Odbija bitmapę w poziomie. @param id Bitmapa.
    void flipHorizontal(BitmapId id);
    /// @brief Odbija bitmapę w pionie. @param id Bitmapa.
    void flipVertical(BitmapId id);
    /**
     * @brief Kopiuje fragment obrazu src na obraz dst (z przycinaniem do granic dst).
     * @param src Obraz źródłowy.
     * @param dst Obraz docelowy.
     * @param dstPos Pozycja lewego górnego rogu fragmentu w dst (może być ujemna).
     * @param rect Fragment src do skopiowania (pusty = cały obraz).
     * @param alphaBlend true - mieszaj kanał alfa, false - nadpisuj piksele.
     */
    static void blit(const Image& src, Image& dst, Vector2i dstPos, const IntRect& rect = {}, bool alphaBlend=false);
    /**
     * @brief Rysuje obraz src na dst z obrotem i skalowaniem (metoda najbliższego sąsiada, z alfa).
     * @param src Obraz źródłowy.
     * @param dst Obraz docelowy.
     * @param center Punkt w dst, w którym znajdzie się środek fragmentu.
     * @param angle Kąt obrotu w radianach (zgodnie z ruchem wskazówek zegara).
     * @param scale Skala w poziomie i w pionie.
     * @param srcRect Fragment src (pusty = cały obraz).
     */
    static void blitTransformed(const Image& src, Image& dst, Vector2u center, float angle = 0.f,Vector2f scale = {1.f, 1.f}, const IntRect& srcRect = {});
};


#endif //GRAFIKA2D_BITMAPHANDLER_H