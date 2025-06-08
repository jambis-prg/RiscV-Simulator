#pragma once

namespace RSCV
{
    class Layer
    {
    public:
        virtual ~Layer()
        {
        }

        virtual void Attach() = 0;
        virtual void Dettach() = 0;

        virtual void OnEvent() = 0;
        virtual void Update() = 0;
    };
}