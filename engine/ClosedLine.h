#ifndef GRAFIKA2D_CLOSEDLINE_H
#define GRAFIKA2D_CLOSEDLINE_H
#include "segmentedLine.h"


/**
 * @file ClosedLine.h
 * @brief Linia łamana domknięta (ostatni punkt łączy się z pierwszym).
 */

/**
 * @brief Łamana, która przy rysowaniu dodatkowo łączy ostatni punkt z pierwszym.
 */
class ClosedLine:public SegmentedLine {
    public:
    /// @brief Rysuje łamaną wraz z odcinkiem zamykającym. @param bufor Obraz docelowy.
    void draw(Image &bufor) override;
};


#endif //GRAFIKA2D_CLOSEDLINE_H