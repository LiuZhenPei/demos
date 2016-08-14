#include "shape.h"
#include "canvas.h"
#include "circle.h"

int main(int argc, char **argv) {
    Canvas *canvas = canvas_new(800, 480, 0xffffff);

    Shape *circle10 = (Shape *)circle_new(10, 10, 10);
    canvas_add(canvas, circle10);

    Shape *circle20 = (Shape *)circle_new(20, 20, 20);
    canvas_add(canvas, circle20);

    /*
     * canvas_add(canvas, line);
     * canvas_add(canvas, rect);
     * ...
     */

    canvas_update(canvas);

    canvas_destroy(canvas);

    shape_destroy(circle10);
    shape_destroy(circle20);

    return 0;
}
