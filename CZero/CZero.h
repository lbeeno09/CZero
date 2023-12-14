#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

class CZeroGUI
{
public:
    CZeroGUI()
    {
        // Setup
        glfwSetErrorCallback(glfw_error_callback);
        if(!glfwInit())
            std::exit(1);

        // Decide GL+GLSL versions
#if defined(__APPLE__)
    // GL 3.2 + GLSL 150
        const char *glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
        window = glfwCreateWindow(1280, 720, "Deep Learning From Scratch", nullptr, nullptr);
        if(window == nullptr)
            std::exit(1);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO &io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle &style = ImGui::GetStyle();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != nullptr);
        ImFont *font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 15.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    }
    
    ~CZeroGUI()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }


	void StartUp()
    {
        // Our state
        show_demo_window = true;
        clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    }

	void Update()
    {
        // Enable Dockspace
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if(show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if(ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // DL Menu
        {
            openContextMenu = false;

            ImGui::Begin("Deep Learning Menu");

            if(ImGui::CollapsingHeader("Principles"))
            {
                if(ImGui::TreeNode("Ch01"))
                {
                    TextWithContextMenu("Ch01", "hungry");
                    TextWithContextMenu("Ch01", "man");
                    TextWithContextMenu("Ch01", "sin_graph");
                    TextWithContextMenu("Ch01", "sin_cos_graph");
                    TextWithContextMenu("Ch01", "img_show");

                    ImGui::TreePop();
                    ImGui::Spacing();
                }
                if(ImGui::TreeNode("Ch02"))
                {


                    ImGui::TreePop();
                    ImGui::Spacing();
                }
                if(ImGui::TreeNode("Ch03"))
                {


                    ImGui::TreePop();
                    ImGui::Spacing();
                }
                if(ImGui::TreeNode("Ch04"))
                {


                    ImGui::TreePop();
                    ImGui::Spacing();
                }
                if(ImGui::TreeNode("Ch05"))
                {


                    ImGui::TreePop();
                    ImGui::Spacing();
                }
                if(ImGui::TreeNode("Ch06"))
                {


                    ImGui::TreePop();
                    ImGui::Spacing();
                }
                if(ImGui::TreeNode("Ch07"))
                {


                    ImGui::TreePop();
                    ImGui::Spacing();
                }
                if(ImGui::TreeNode("Ch08"))
                {


                    ImGui::TreePop();
                    ImGui::Spacing();
                }

            }
            if(ImGui::CollapsingHeader("NLP"))
            {

            }
            if(ImGui::CollapsingHeader("Framework"))
            {

            }
            if(ImGui::CollapsingHeader("RL"))
            {

            }

            ImGui::End();
        }

        // Plot window
        {
            int bar_data[11] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

            ImGui::Begin("Output Window");
            if(ImGui::BeginTabBar("Tabs"))
            {
                if(ImGui::BeginTabItem("Output"))
                {
                    ImGui::TextWrapped(fileContent.c_str());

                    ImGui::EndTabItem();
                }
                if(ImGui::BeginTabItem("Plot"))
                {
                    if(ImPlot::BeginPlot("My Plot"))
                    {
                        ImPlot::PlotBars("My Bar Plot", bar_data, 11);

                        ImPlot::EndPlot();
                    }

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }

	void Run()
    {
        // State Initialize
        StartUp();

        // Main loop
        while(!glfwWindowShouldClose(window))
        {
            // Poll and handle events (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            Update();

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow *backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            glfwSwapBuffers(window);
        }
    }

    void TextWithContextMenu(std::string chapter, std::string label);
    void readSourceCode();
    void readExecutable();
protected:
	ImVec4 clear_color;
private:
	GLFWwindow *window = nullptr;

	bool show_demo_window = true;
    bool openContextMenu = false;
    std::string filepath = "";
    std::string fileContent = "";
};

void CZeroGUI::TextWithContextMenu(std::string chapter, std::string file)
{
    ImGui::BulletText(file.c_str());

    if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup(file.c_str());
        openContextMenu = true;
    }
    if(ImGui::BeginPopupContextItem(file.c_str()))
    {
        std::string prompt = "Show " + file + ".cpp";
        if(ImGui::MenuItem(prompt.c_str()))
        {
            filepath = "..\\Principles/" + chapter + "\\" + file + "\\main.cpp";

             readSourceCode();

            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
        filepath = "..\\Principles\\" + chapter + "\\" + file + "\\" + file + ".exe";

        readExecutable();
    }
}

void CZeroGUI::readSourceCode()
{
    std::ifstream file(filepath);
    if(file)
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        fileContent = content;
    }
    else
    {
        fileContent = "Error: Unable to call source code";
    }
}

void CZeroGUI::readExecutable()
{
    std::string path = std::filesystem::current_path().generic_string() + "\\" + filepath;

    std::string result;
    FILE *pipe = _popen(path.c_str(), "r");
    if(pipe)
    {
        char buffer[128];
        while(!feof(pipe))
        {
            if(fgets(buffer, sizeof(buffer), pipe) != nullptr)
            {
                result += buffer;
            }
        }
        _pclose(pipe);

        fileContent = result;
    }
    else
    {
        fileContent = "Error: Unable to execute executable";
    }
}