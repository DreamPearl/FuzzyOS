// desktop environment
#include <iostream.h>
#include <vector.h>
#include <conio.h>
#include <utility.h>
#include <graphics.h>
#include <dirent.h>

const int WINDOW_HEIGHT = GRAPHICS_MAX_HEIGHT;
const int WINDOW_WIDTH  = GRAPHICS_MAX_WIDTH;

enum IconType {
    DATA_FILE = 0,
    EXECUTABLE = 1
};

struct IconData {
    std::string fname;
    IconType type;
};

class GraphicsModeController {
    int gd, gm;
    bool is_graphics_enabled;
public:

    GraphicsModeController() : is_graphics_enabled(false) {
        // assume to be in text-mode at start
    }

    void graphics_mode_enable() {
        if (is_graphics_enabled) return;
        gd = DETECT;

        std::graphics::initgraph(&gd, &gm, NULL);
        int gerr = std::graphics::graphresult();
        if (gerr != 0) {
            std::cout << "failed to open graphics mode, err: " <<  gerr << std::endl;
            std::exit(1);
        }
        is_graphics_enabled = true;
    }

    void graphics_mode_disable() {
        if (!is_graphics_enabled) return;

        std::graphics::closegraph();
        is_graphics_enabled = false;
    }
} gmcontroller;

class Desktop {
    const int ICON_WIDTH = 48;
    const int ICON_HEIGHT = ICON_WIDTH+8;
    const int ICON_COUNT_HOR = WINDOW_WIDTH / ICON_WIDTH;

    int width, height;
    std::vector<IconData> files;

    int text_height;
    int text_width;
    int cursor_id;

public:
    Desktop(int width, int height) : width(width), height(height) {
        text_width = std::graphics::textwidth(" ");
        text_height = std::graphics::textheight(" ");

        cursor_id = 0;
        gmcontroller.graphics_mode_enable();
    }

    void refresh() {
        struct std::DIR dir;
        std::opendir(&dir);
        struct std::dirent *dp;

        files.clear();
        while ((dp = std::readdir(&dir)) !=NULL) {
            bool executable = dp->flag & std::DIRENT_EXECUTABLE;
            IconData file{
                .fname = std::string(dp->d_name),
                .type = executable?EXECUTABLE:DATA_FILE,
            };
            files.push_back(file);
        }
        draw();
    }

    void launch_app(int app_id) {
        // switch to text-mode
        gmcontroller.graphics_mode_disable();

        // TODO: spawnv
        if (files[app_id].type == EXECUTABLE) {
        } else {
            // execute cat <filename>
        }

        // switch back to graphics mode
        gmcontroller.graphics_mode_enable();
    }

    void controller(char c) {
        // assumes at least one file.
        int id_count = files.size();

        switch (c) {
            case 'd':
                cursor_id = (cursor_id+1)%id_count;
                break;
            case 'a':
                cursor_id = (cursor_id+id_count-1)%id_count;
                break;
            case 'w':
                cursor_id = (cursor_id-ICON_COUNT_HOR+id_count)%id_count;
                break;
            case 's':
                cursor_id = (cursor_id+ICON_COUNT_HOR)%id_count;
                break;
            case 13:
                // selected
                launch_app(cursor_id);
                break;
        }
    }

    void execute() {
        do {
            refresh();
            controller(std::getch());
        } while(1);

    }

    void draw_icon(int id, const IconData &file) {
        int x = (id % ICON_COUNT_HOR) * ICON_WIDTH;
        int y = (id / ICON_COUNT_HOR) * ICON_HEIGHT;

        const int half_border = 1;

        const int max_length = (ICON_WIDTH-half_border*2)/text_width;
        std::string fname = file.fname;
        if (fname.length() > max_length) {
            fname[max_length] = '\0';
        }

        // highlight selected icon
        if (id == cursor_id) {
            std::graphics::setcolor(YELLOW);
            std::graphics::bar(x, y, x+ICON_WIDTH-1, y+ICON_HEIGHT-1);
        }

        // draw icon and text
        if (file.type == EXECUTABLE) {
            std::graphics::setcolor(GREEN);
        } else {
            // data file
            std::graphics::setcolor(BROWN);
        }
        std::graphics::bar(x+half_border, y+half_border, x+ICON_WIDTH-1-half_border, y+ICON_HEIGHT-half_border-text_height-1);
        std::graphics::setcolor(BLACK);
        std::graphics::outtextxy(x, y+ICON_HEIGHT-text_height-half_border, fname.c_str());
    }

    void draw() {
        std::graphics::setcolor(WHITE);
        std::graphics::bar(0, 0, this->width-1, this->height-1);

        for(int i=0; i < files.size(); i++) {
            draw_icon(i, files[i]);
        }

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
    gmcontroller.graphics_mode_disable();
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
    // graphics started
    std::atexit(cleanup_graphics);
    start_dm();
    return 0;
}
