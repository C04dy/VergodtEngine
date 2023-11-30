#include <pybind11/pybind11.h>
#include "Objects/Node.h"

namespace py = pybind11;

PYBIND11_MODULE(VergodtEngine, m){
    py::class_<Vector2>(m, "Vector2")
        .def("SetX", &Vector2::SetX)
        .def("SetY", &Vector2::SetY)
        .def("GetX", &Vector2::GetX)
        .def("GetY", &Vector2::GetY);

    py::class_<TransformClass>(m, "TransformClass")
        .def_readwrite("Position", &TransformClass::Position)
        .def_readwrite("Size", &TransformClass::Size)
        .def_readwrite("Angle", &TransformClass::Angle);
    
    py::class_<Node>(m, "Node")
        .def_readwrite("Transform", &Node::Transform)
        .def_readwrite("Name", &Node::Name);
}