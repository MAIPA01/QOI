#include <iostream>
#include "include/Model/qoi.h"
#include "include/Exceptions/qoi_exceptions.h"
#include "PixelViewer.h"
#include "Pixel_Exceptions.h"
#include "SimpleGameEngine.h"
#include "EngineTestClass.h"

using namespace std;

/*string CharToBytes(unsigned char character) {
    string value = "";
    string zeros = "";
    string reverse = "";
    while (character != 0) {
        reverse += ((character % 2) + '0');
        character >>= 1;
    }
    for (int i = 0; i < reverse.length(); i++) {
        value += reverse[reverse.length() - i - 1];
    }

    if (value.length() != 8) {
        for (int i = 0; i < 8 - value.length(); i++) {
            zeros += '0';
        }
    }
    return zeros + value;
}

string FileBytes(string path) {
    fstream file(path, ios_base::in | ios_base::binary);
    string value = "";
    char byte = 0;

    while (file.read(&byte, sizeof(char))) {
        value += CharToBytes(byte);
    }

    file.close();

    return value;
}*/

int main(int argc, char* args[])
{   
    // QOI:
    /*string fileName = "./qoi_test_images/dice.qoi";
    cout << "Openning " << fileName << " file..." << endl;
    QOI qoi_image;
    try{
        qoi_image.decode(fileName);
        cout << "Finished." << endl;
    }
    catch (Exception &e){
        cerr << e.GetMessage() << endl;
    }*/

    // SDL:
    /*PixelViewer pixelView;

    cout << "Creating Window..." << endl;
    bool isCreated = false;
    try {
        pixelView.Init("QOI Image View", qoi_image.getWidth(), qoi_image.getHeight());
        isCreated = true;
        cout << "Created" << endl;
    }
    catch (PixelViewerException& e) {
        cerr << e.GetMessage() << endl;
    }

    if (!isCreated)
        return 0;

    pixelView.SetPixels(qoi_image.getPixels(), qoi_image.getWidth() * qoi_image.getHeight());
    pixelView.LoadMedia();

    cout << "Running..." << endl;
    pixelView.EventLoop();

    pixelView.Close();
    cout << "Ended" << endl;*/

    /*PixelViewer view;
    view.Init("SDL TUTORIAL", 640, 480);*/

    EngineTestClass test;
    test.Init("Test", 640, 480);

    return 0;
}
