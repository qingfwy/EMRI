#pragma once
#include <implot.h>
#include <math.h>
#include "mtx.h"
#include "mymath.h"

namespace ui
{
    void create_switch_button(bool& flag, const char* str[])
    {
        if (!flag)
        {
            if (ImGui::Button(str[0]))
            {
                flag = !flag;
            }
        }
        else
        {
            if (ImGui::Button(str[1]))
            {
                flag = !flag;
            }
        }
    }

    template<typename T>
    class UI
    {
    public:
        UI(T& t) : obj(t)
        {
            logsigma = t.get_logsigma();
        }

        void plot(const ScrollBuffer& sb1, const ScrollBuffer& sb2, const ScrollBuffer& sl)
        {
            static bool p_open = true;
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            ImGui::Begin("EKL");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("t = %.1f", sb1.Data[0].x);
            if (ImPlot::BeginPlot("e1e2", ImVec2(600, 300)))
            {
                ImPlot::SetupAxesLimits(0, 1e8, 0, 1);
                //ImPlot::PlotLine("e1", &sb1.Data[0].x, &sb1.Data[0].y, sb1.Data.size(), 0, sb1.Offset, sizeof(Vec2));
                //ImPlot::PlotLine("e2", &sb2.Data[0].x, &sb2.Data[0].y, sb2.Data.size(), 0, sb2.Offset, sizeof(Vec2));
                ImPlot::PlotScatter("e1", &sb1.Data[0].x, &sb1.Data[0].y, sb1.Data.size(), 0, sb1.Offset, sizeof(Vec2));
                ImPlot::PlotScatter("e2", &sb2.Data[0].x, &sb2.Data[0].y, sb2.Data.size(), 0, sb2.Offset, sizeof(Vec2));
                ImPlot::EndPlot();
            }
            if (ImPlot::BeginPlot("step length", ImVec2(600, 300)))
            {
                ImPlot::SetupAxesLimits(0, 1e8, 0, 50000);
                ImPlot::PlotScatter("sl", &sl.Data[0].x, &sl.Data[0].y, sl.Data.size(), 0, sl.Offset, sizeof(Vec2));
                ImPlot::EndPlot();
            }
            ImGui::SameLine();
            ImGui::Text("total relative error = %.3e", pow(10, logsigma) * obj.get_steps());
            if (ImGui::InputInt("sigma(10^x)", &logsigma))
            {
                reset();
                std::lock_guard<std::mutex> lk(MTX::mtx);
                obj.set_logsigma(logsigma);
            }

            if (ImGui::Button("reset"))
            {
                reset();
            }
            ImGui::SameLine(200);
            create_switch_button(start, startorstop);

            ImGui::End();
        }

        bool runornot()
        {
            return start;
        }

        bool want_reset()
        {
            return wantreset;
        }

        void reset()
        {
                start = false;
                wantreset = true;
        }
        
        void reset_finish()
        {
            wantreset = false;
        }

    private:
        T& obj;
        bool start = false, wantreset = false, showp2ori = false;
        const char* startorstop[2] = { "start", "stop" };
        int logsigma;
    };
}
