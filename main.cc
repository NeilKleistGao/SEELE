/**
 * Copyright (C) 2007 Free Software Foundation, Inc.
 * https://fsf.org/ Everyone is permitted to copy and distribute verbatim copies of this license document, but changing it is not allowed.
 * The GNU General Public License is a free, copyleft license for software and other kinds of works.
 * The licenses for most software and other practical works are designed to take away your freedom to share and change the works.
 * By contrast, the GNU General Public License is intended to guarantee your freedom to share and change all versions of a program--to make sure it remains free software for all its users.
 * We, the Free Software Foundation, use the GNU General Public License for most of our software;
 * it applies also to any other work released this way by its authors. You can apply it to your programs, too.
 */

/// @file main.cc

#include <chrono>
#include <iomanip>
#include <thread>
#include <atomic>

#include "cxxopts/cxxopts.hpp"
#include "core/general/renderer.h"
#include "core/rasterization/rasterization_renderer.h"
#include "core/raytracing/raytracing_renderer.h"
#include "core/photon/photon_renderer.h"

static core::general::Renderer* render;
static std::atomic<bool> finished;

void monitor() {
    while (!finished.load()) {
        auto process = render->getProcess() * 100;
        printf("\rRendering[%.2f%%]: ", process);

        for (int i = 0; i <= process; i++) {
            printf("█");
        }
        puts("");

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(300ms);
    }
}

int main(int argc, char* argv[]) {
    cxxopts::Options options{"SEELE", "Software-rEndEring Laboratorial Engine"};

    options.add_options()
    ("s,script", "Rendering Script", cxxopts::value<std::string>())
    ("m,method", "Rendering Method", cxxopts::value<std::string>()->default_value("rasterization"))
    ("w,width", "Image Width", cxxopts::value<int>()->default_value("1024"))
    ("h,height", "Image Height", cxxopts::value<int>()->default_value("768"))
    ("f,filename", "Output Filename", cxxopts::value<std::string>()->default_value("output.jpg"));

    try {
        auto result = options.parse(argc, argv);
        std::string method = result["method"].as<std::string>();
        std::string script_path = result["script"].as<std::string>();
        int width = result["width"].as<int>(),
            height = result["height"].as<int>();
        std::string output_file = result["filename"].as<std::string>();

        auto begin = std::chrono::system_clock::now();
        if (method == "rasterization") {
            render = new core::rasterization::RasterizationRenderer(script_path, output_file, width, height);
        }
        else if (method == "raytracing") {
            render = new core::raytracing::RaytracingRenderer(script_path, output_file, width, height);
        }
        else if (method == "photon") {
            render = new core::photon::PhotonRenderer(script_path, output_file, width, height);
        }
        else {
            throw std::exception{};
        }

        finished.store(false);
        std::thread monitor_thread{monitor};
        render->render();
        finished.store(true);
        monitor_thread.join();

        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> diff = end - begin;
        double length = diff.count();
        std::cout << "rendering finished, using " << std::setw(4) << length << "s." << std::endl;

        delete render;
        render = nullptr;
    }
    catch (...) {
        std::cerr << options.help() << std::endl;
    }

    return 0;
}