// desktop environment
#include <iostream.h>
#include <conio.h>
#include <graphics.h>

class Desktop {
    int width, height;
public:
    Desktop(int width, int height) : width(width), height(height) {
    }

    void execute() {
        do {
            draw();
            std::getch();
        } while(1);

    }

    void draw() {
        std::graphics::setcolor(WHITE);
        std::graphics::bar(0, 0, this->width-1, this->height-1);
        std::graphics::setcolor(BLACK);

        std::graphics::graphflush();
    }
};

void start_dm() {
    std::graphics::graphautoflush_disable();
    std::graphics::setbkcolor(BLACK);

    Desktop d(GRAPHICS_MAX_WIDTH, GRAPHICS_MAX_HEIGHT);
    d.execute();
}

void cleanup_graphics() {
    std::graphics::closegraph();
    std::cout << "desktop environment graphics closed" << std::endl;
    std::cout << "heap memory usage at exit " <<
        (std::benchmark_get_heap_usage()) << " bytes" << std::endl;
    std::cout << "heap memory area at exit " <<
        (std::benchmark_get_heap_area()) << " bytes" << std::endl;
}

int show_usage() {
    std::cout << "desktop environment" << std::endl;
    return 0;
}

int main(int argc,char *argv[]) {
    if (argc != 1) {
        // assumes user wants desktop --help
        return show_usage();
    }

    int gd = DETECT, gm;
    std::graphics::initgraph(&gd, &gm, NULL);
    int gerr = std::graphics::graphresult();
    if (gerr != 0) {
        std::cout << "failed to open graphics mode, err: " <<  gerr << std::endl;
        return 1;
    }
    // graphics started
    std::atexit(cleanup_graphics);
    start_dm();
    return 0;
}
