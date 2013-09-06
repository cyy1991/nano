# NanoCV

**NanoCV** is a small (nano) C++(11) library that implements various (non-linear) optimization and machine learning algorithms. The library is used as a sandbox for implementing and testing various image classification and object detection methods.

As the library evolves, command line programs will be supplied for testing various components and for training and testing models.

The library is written to be cross-platform having only Boost, Eigen and Qt as dependencies.

## Concepts

The library is built around several key concepts mapped to C++ object interfaces. Each instantiation has a string ID associated with which it can be retrieved 
from command line arguments. The main concepts are the following:

* **task** - describes a classification or regression task organized in folds. Each fold contains separate training and test image patches with associated target 
output if any. This concept maps known machine learning and computer vision benchmarks to a common interface. Examples: 

	* **MNIST** - digit classification, 28x28 grayscale inputs

	* **CIFAR-10** - 10-class object classification, 32x32 RGB inputs

	* **CMU-FACES** - face detection (binary classification), 19x19 grayscale inputs

	* **STL-10** - 10-class object classification, 96x96 RGB inputs

* **loss** - describes 

* **model** - describes

* **layer** - describes

* **trainer**


 
