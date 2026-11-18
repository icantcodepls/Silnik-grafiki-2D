#ifndef GRAFIKA2D_BITMAPOBJECT_H
#define GRAFIKA2D_BITMAPOBJECT_H
#include "BitmapHandler.h"
#include "DrawableObject.h"
#include "TransformableObject.h"

/**
 * @file BitmapObject.h
 * @brief Obiekt graficzny złożony z fragmentów bitmap.
 */

/**
 * @brief Jedna część obiektu bitmapowego: fragment bitmapy z własnym przesunięciem, obrotem i skalą.
 */
struct BitmapPart {
    BitmapId bitmap=INVALID_BITMAP;   ///< Bitmapa źródłowa.
    IntRect source{};                 ///< Fragment bitmapy (pusty = cała).
    Vector2u offset{0,0};             ///< Przesunięcie względem pozycji obiektu (px).
    float rotation=0.0f;              ///< Obrót części względem obiektu (rad).
    Vector2f scale={1.0f,1.0f};       ///< Skala części.
    string name;                      ///< Nazwa części (opcjonalna).
};

/**
 * @brief Obiekt rysowany z bitmap przechowywanych w BitmapHandler.
 *
 * Składa się z listy części (BitmapPart). Całość ma pozycję, obrót i skalę, a każda
 * część może dodatkowo mieć własny obrót i skalę, dzięki czemu można np. składać postać
 * z nóg i tułowia obracanych niezależnie.
 */
class BitmapObject:public DrawableObject{
protected:
    BitmapHandler* handler;       ///< Magazyn bitmap (bez własności).
    vector<BitmapPart> parts;     ///< Części obiektu, rysowane w kolejności dodania.
    Vector2u position{0,0};       ///< Pozycja obiektu w buforze.
    float rotation=0.0f;          ///< Obrót całego obiektu (rad).
    float scaleFactor=1.0f;       ///< Skala całego obiektu.
public:
    /// @brief Tworzy pusty obiekt. @param handler Magazyn bitmap; musi żyć dłużej niż obiekt.
    BitmapObject(BitmapHandler& handler);
    /**
     * @brief Dodaje część obiektu.
     * @param bitmap Bitmapa źródłowa.
     * @param offset Przesunięcie części względem pozycji obiektu.
     * @param source Fragment bitmapy (pusty = cała).
     * @param name Nazwa części.
     * @return Indeks dodanej części.
     */
    size_t addPart(BitmapId bitmap, Vector2u offset = {0,0}, const IntRect& source = {}, const string& name = "");
    /// @brief Zwraca część o danym indeksie. @param index Indeks części.
    BitmapPart& getPart(size_t index){return parts[index];}
    /// @brief Zwraca liczbę części.
    size_t getPartCount(){return parts.size();}
    /// @brief Rysuje wszystkie części w buforze. @param bufor Obraz docelowy.
    void draw(Image &bufor) override;
    /// @brief Przesuwa obiekt. @param dx Przesunięcie poziome. @param dy Przesunięcie pionowe.
    void translate(int dx, int dy);
    /// @brief Zwiększa obrót obiektu. @param angle Kąt w radianach.
    void rotate(float angle);
    /// @brief Mnoży skalę obiektu. @param k Współczynnik.
    void scale(float k);
    /// @brief Ustawia pozycję. @param p Pozycja w buforze.
    void setPosition(Vector2u p){position=p;}
    /// @brief Ustawia obrót. @param r Kąt w radianach.
    void setRotation(float r){rotation=r;}
    /// @brief Ustawia skalę. @param s Skala.
    void setScale(float s){scaleFactor=s;}
    /// @brief Zwraca pozycję.
    Vector2u getPosition(){return position;}
    /// @brief Zwraca obrót (rad).
    float getRotation(){return rotation;}
    /// @brief Zwraca skalę.
    float getScale(){return scaleFactor;}
};


#endif //GRAFIKA2D_BITMAPOBJECT_H