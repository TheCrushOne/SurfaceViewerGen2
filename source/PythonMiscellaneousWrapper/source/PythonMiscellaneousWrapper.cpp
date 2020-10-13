#include "stdafx.h"

#include "crossdllinterface/PythonWrapperInterface.h"
#include "PythonMiscellaneousWrapper.h"

#include <Python.h>
#include <pybind11/pybind11.h>

using namespace python_wrapper;
namespace py = pybind11;

struct Pet {
   Pet(const std::string& name) : name(name) { }
   void setName(const std::string& name_) { name = name_; }
   const std::string& getName() const { return name; }

   std::string name;
};

PYBIND11_MODULE(example, m) {
   py::class_<Pet>(m, "Pet")
      .def(py::init<const std::string&>())
      .def("setName", &Pet::setName)
      .def("getName", &Pet::getName);
}

PythonMiscellaneousWrapper::PythonMiscellaneousWrapper(central_pack* pack)
   : Central(pack)
{}

PythonMiscellaneousWrapper::~PythonMiscellaneousWrapper()
{}

void PythonMiscellaneousWrapper::RunScript()
{
   PyObject* pInt;

   Py_Initialize();

   PyRun_SimpleString("print('Hello World from Embedded Python!!!')");

   Py_Finalize();
}

extern "C" PWEXPRTIMPRT iPythonWrapper * CreatePythonWrapper(central_pack * pack)
{
   return new PythonMiscellaneousWrapper(pack);
}