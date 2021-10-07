// desktop environment
#include <iostream.h>
#include <vector.h>
#include <conio.h>
#include <graphics.h>
#include <dirent.h>

class Desktop {
    int width, height;
    std::vector<std::string> apps;  // executables
    std::vector<std::string> data_files;  // non-executables

    int text_height;

public:
    Desktop(int width, int height) : width(width), height(height) {
        text_height = std::graphics::textheight(" ");
    }

    void refresh() {
        struct std::DIR dir;
        std::opendir(&dir);
        struct std::dirent *dp;

        apps.clear();
        data_files.clear();
        while ((dp = std::readdir(&dir)) !=NULL) {
            bool executable = dp->flag & std::DIRENT_EXECUTABLE;
            if (executable) {
                // apps.push_back(std::string("yoyo1234"));
                apps.push_back(std::string(dp->d_name));
                std::cout<<" START: "<< (int)(apps[0].c_str()) <<std::endl;
                for(int i=0;i<apps.size();i++) {
                    std::cout<< (int)&(apps[i]) << ',';
                }
                std::cout<<"\n";
                for(int i=0;i<apps.size();i++) {
                    std::cout<< (apps[i]) << ',';
                }
                std::cout<<"\n";
                for(int i=0;i<apps.size();i++) {
                    std::cout<< (int)(apps[i].c_str()) << ',';
                }
                std::cout<<"\n";
                std::getch();
            } else {
                data_files.push_back(std::string(dp->d_name));
            }
        }
        draw();
    }

    void execute() {
        refresh();
        do {
            // draw();
            std::getch();
            break;
        } while(1);

    }

    void draw() {
        std::graphics::setcolor(WHITE);
        std::graphics::bar(0, 0, this->width-1, this->height-1);

        int _x = 0;
        int _y = 0;
        std::graphics::setcolor(BLACK);
        std::string m_apps("Apps");
        std::graphics::outtextxy(_x, _y, m_apps.c_str());
        _y += text_height;
        std::graphics::setcolor(GREEN);
        for(int i=0; i < apps.size(); i++) {
            // std::cout << apps[i] << std::endl;

            std::graphics::outtextxy(_x, _y, apps[i].c_str());
            _y += text_height;
        }

        std::graphics::setcolor(BLACK);
        std::string m_datafiles("Data files");
        std::graphics::outtextxy(_x, _y, m_datafiles.c_str());
        _y += text_height;
        std::graphics::setcolor(BLACK);
        for(int i=0; i < data_files.size(); i++) {
            std::graphics::outtextxy(_x, _y, data_files[i].c_str());
            _y += text_height;
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
    // std::graphics::initgraph(&gd, &gm, NULL);
    // int gerr = std::graphics::graphresult();
    // if (gerr != 0) {
    //     std::cout << "failed to open graphics mode, err: " <<  gerr << std::endl;
    //     return 1;
    // }
    // graphics started
    std::atexit(cleanup_graphics);
    start_dm();
    return 0;
}
