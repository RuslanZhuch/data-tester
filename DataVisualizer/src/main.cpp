#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <GLFW/glfw3.h>

#include "data-holder.h"
#include "data-store.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------

struct ChannelData
{
    std::string        name;
    std::vector<float> refTimes;
    std::vector<float> refVals;
    std::vector<float> testTimes;
    std::vector<float> testVals;
};

static std::vector<ChannelData> buildChannels(const DataHolder& ref, const DataHolder& test)
{
    std::map<std::string, ChannelData> channelMap;

    ref.forEachDataRange([&](std::string_view name, const DataRange<float>& range) {
        auto& ch     = channelMap[std::string(name)];
        ch.name      = name;
        ch.refTimes  = range.timeSeconds;
        ch.refVals   = range.data;
    });

    test.forEachDataRange([&](std::string_view name, const DataRange<float>& range) {
        auto& ch      = channelMap[std::string(name)];
        ch.name       = name;
        ch.testTimes  = range.timeSeconds;
        ch.testVals   = range.data;
    });

    std::vector<ChannelData> result;
    for (auto& [name, ch] : channelMap)
        result.push_back(std::move(ch));
    return result;
}

// ---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::fprintf(stderr,
            "Usage: DataVisualizer <reference.sent> <test.sent>\n");
        return 1;
    }

    const std::filesystem::path refPath  = argv[1];
    const std::filesystem::path testPath = argv[2];

    DataHolder ref, test;
    DataStoreBinary store;

    if (!store.loadData(refPath, ref))
    {
        std::fprintf(stderr, "Failed to load reference file: %s\n", argv[1]);
        return 1;
    }
    if (!store.loadData(testPath, test))
    {
        std::fprintf(stderr, "Failed to load test file: %s\n", argv[2]);
        return 1;
    }

    const auto channels = buildChannels(ref, test);
    if (channels.empty())
    {
        std::fprintf(stderr, "No matching channels found between the two files.\n");
        return 1;
    }

    // Window title shows the two file names
    const std::string title = refPath.filename().string() + "  vs  " + testPath.filename().string();

    // -----------------------------------------------------------------------
    // GLFW + ImGui + ImPlot setup
    // -----------------------------------------------------------------------
    glfwSetErrorCallback([](int, const char* desc) {
        std::fprintf(stderr, "GLFW: %s\n", desc);
    });

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 800, title.c_str(), nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // -----------------------------------------------------------------------
    // Render loop
    // -----------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Full-screen host window
        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->Pos);
        ImGui::SetNextWindowSize(vp->Size);
        ImGui::Begin("##host", nullptr,
            ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove      | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImGui::TextDisabled("%s", title.c_str());
        ImGui::Separator();

        const int    nCh     = static_cast<int>(channels.size());
        const float  spacing = ImGui::GetStyle().ItemSpacing.y;
        const float  headerH = ImGui::GetCursorPosY();
        const float  availH  = ImGui::GetContentRegionAvail().y;
        const float  plotH   = (availH - spacing * (nCh - 1)) / static_cast<float>(nCh);
        const ImVec2 plotSize{ -1.0f, plotH };

        for (const auto& ch : channels)
        {
            if (ImPlot::BeginPlot(ch.name.c_str(), plotSize))
            {
                ImPlot::SetupAxes("Time (s)", ch.name.c_str());
                ImPlot::PlotLine(refPath.filename().string().c_str(),
                    ch.refTimes.data(),  ch.refVals.data(),  static_cast<int>(ch.refVals.size()));
                ImPlot::PlotLine(testPath.filename().string().c_str(),
                    ch.testTimes.data(), ch.testVals.data(), static_cast<int>(ch.testVals.size()));
                ImPlot::EndPlot();
            }
        }

        // suppress unused-variable warning for headerH
        (void)headerH;

        ImGui::End();

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
