/*
ESCUELA POLITECNICA NACIONAL SEDE CUENCA
DOCTORADO EN CIENCIAS COMPUTACIONALES
MODULO DE VISION POR COMPUTADOR
REALIZADO POR:  ANDRES GORDON GARCES
Esta aplicación permite incorporar dos videos, ya sean capturas
de cámara o videos almacenados, en una imagen que sirve de plantilla
para hacer cualquier tipo de Streaming, empleando librerías de OpenCV
*/
#include <windows.h>
#include <conio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// DEFINICION DE UN VECTOR PARA ALMACENAR COORDENADAS DE PUNTOS PARA POLIGONOS
vector <Point2f> puntosPoligonoCam;
bool puntosSeleccionadosCam = false;
vector <Point2f> puntosPoligonoVid;
bool puntosSeleccionadosVid = false;

// VARIABLES BOOLEANAS PARA CREAR BOTONES PARA APLICACION DE FILTROS
// ECUALIZACIÓN NORMAL DE HISTOGRAMA 
bool aplica_ecualizacion_cam = false;
bool aplica_ecualizacion_vid = false;

// ECUALIZACION CLAHE DE HISTOGRAMA
bool aplica_clahe_cam = false;
bool aplica_clahe_vid = false;

// OPERACION MORFOLOGICA DE APERTURA
bool aplica_morfologica_cam = false;
bool aplica_morfologica_vid = false;

// SUAVIZADO BILATERAL
bool aplica_bilateral_cam = false;
bool aplica_bilateral_vid = false;

// CREACION DE OBJETO CLAHE PARA MANEJAR LA MEMORIA
cv :: Ptr<cv :: CLAHE> clahe = cv :: createCLAHE(2.0, cv :: Size(8,8));

// FUNCION PARA UTILIZAR LOS EVENTOS DEL RATON, CLIC IZQUIERDO
void mouseCallbackCam(int eventCam, int xCam, int yCam, int flagsCam, void* userdataCam){
    if (eventCam == EVENT_LBUTTONDOWN){
        puntosPoligonoCam.emplace_back(Point2f(xCam, yCam));
        cout << "Punto camara agregado: " << xCam << ", " << yCam << endl;
    }
}
void mouseCallbackVid(int eventVid, int xVid, int yVid, int flagsVid, void* userdataVid){
    if (eventVid == EVENT_LBUTTONDOWN){
        puntosPoligonoVid.emplace_back(Point2f(xVid, yVid));
        cout << "Punto video agregado: " << xVid << ", " << yVid << endl;
    }
}

// INICIO PROGRAMA PRINCIPAL
int main(){
    VideoCapture capCam(0);
    string rutaVideo = "C:\\DOCUMENTOS\\MAYO - SEPTIEMBRE 2024\\DOCTORADO UPS CIENCIAS COMPU\\VISIÓN POR COMPUTADOR\\PROYECTO FINAL\\InkGirl.mp4";
    VideoCapture capVid(rutaVideo);

    if (!capCam.isOpened()){
        cerr << "Error: No se puede acceder a la cámara." << endl;
        return -1;
    }
    if (!capVid.isOpened()){
        cerr << "Error: No se puede importar el video." << endl;
        return -1;
    }

    string rutaImagen = "plantilla3.jpg";
    Mat imagenOriginal = imread(rutaImagen); // Cargar la imagen original para no modificarla directamente

    if (imagenOriginal.empty()){
        cerr << "Error: No se puede cargar la imagen." << endl;
        return -1;
    }

    namedWindow("Imagen con Videos", WINDOW_AUTOSIZE);

    Mat imagenDisplay = imagenOriginal.clone(); // Clonar para dibujar puntos temporalmente
    resize(imagenDisplay, imagenDisplay, Size(), 0.4, 0.4); // Redimensionar solo para display inicial

    //Muestra la imagen una vez para que la ventana sea visible antes del mensaje
    imshow("Imagen con Videos", imagenDisplay);
    waitKey(1);     // Pequeño delay para que la ventana se dibuje

    setMouseCallback("Imagen con Videos", mouseCallbackCam);
    MessageBox(NULL,
        "Selecciona con tu clic izquierdo la region para mostrar la camara.\nCuando tengas al menos 3 puntos, presiona ENTER",
        "Dibujar Region de la Camara", MB_OK);

    while (!puntosSeleccionadosCam){
        Mat currentImageForDrawing = imagenDisplay.clone(); // Clonar para dibujar y no acumular dibujos
        // Dibuja puntos para el poligono de la camara
        for (size_t i = 0; i < puntosPoligonoCam.size(); ++i) {
            circle(currentImageForDrawing, puntosPoligonoCam[i], 5, Scalar(233, 0, 245), FILLED);
            if (i > 0) {
                line(currentImageForDrawing, puntosPoligonoCam[i-1], puntosPoligonoCam[i], Scalar(233, 0, 245), 2);
            }
        }

        imshow("Imagen con Videos", currentImageForDrawing);

        int key = waitKey(30);
        if (key == 27) return 0; // ESC para salir
        if (key == 13 || key == 10){ // ENTER para terminar
            if (puntosPoligonoCam.size() >= 3) {
                line(currentImageForDrawing, puntosPoligonoCam.back(), puntosPoligonoCam.front(), Scalar(233, 0, 245), 2);
                imshow("Imagen con Videos", currentImageForDrawing); // Mostrar el polígono cerrado
                puntosSeleccionadosCam = true;
            } else {
                MessageBox(NULL, "Necesitas al menos 3 puntos para formar un polígono.", "Advertencia", MB_OK | MB_ICONWARNING);
            }
        }
    }

    resize(imagenOriginal, imagenOriginal, imagenDisplay.size());
    
    setMouseCallback("Imagen con Videos", mouseCallbackVid);
    MessageBox(NULL,
        "Selecciona con tu clic izquierdo la region para mostrar el video.\nCuando tengas al menos 3 puntos, presiona ENTER",
        "Dibujar Region del Video", MB_OK);

    while (!puntosSeleccionadosVid){
        Mat currentImageForDrawing = imagenOriginal.clone(); // Clonar de la imagen ya con el polígono de cámara
        // Redibujar el polígono de la cámara
        for (size_t i = 0; i < puntosPoligonoCam.size(); ++i) {
            circle(currentImageForDrawing, puntosPoligonoCam[i], 5, Scalar(233, 0, 245), FILLED);
            if (i > 0) {
                line(currentImageForDrawing, puntosPoligonoCam[i-1], puntosPoligonoCam[i], Scalar(233, 0, 245), 2);
            }
        }
        line(currentImageForDrawing, puntosPoligonoCam.back(), puntosPoligonoCam.front(), Scalar(233, 0, 245), 2);


        // Dibuja puntos para el poligono del video
        for (size_t j = 0; j < puntosPoligonoVid.size(); ++j) {
            circle(currentImageForDrawing, puntosPoligonoVid[j], 5, Scalar(233, 234, 0), FILLED);
            if (j > 0) {
                line(currentImageForDrawing, puntosPoligonoVid[j-1], puntosPoligonoVid[j], Scalar(233, 234, 0), 2);
            }
        }

        imshow("Imagen con Videos", currentImageForDrawing);

        int key = waitKey(30);
        if (key == 27) return 0; // ESC para salir
        if (key == 13 || key == 10){ // ENTER para terminar
            if (puntosPoligonoVid.size() >= 3) {
                line(currentImageForDrawing, puntosPoligonoVid.back(), puntosPoligonoVid.front(), Scalar(233, 234, 0), 2);
                imshow("Imagen con Videos", currentImageForDrawing); // Mostrar el polígono cerrado
                puntosSeleccionadosVid = true;
            } else {
                MessageBox(NULL, "Necesitas al menos 3 puntos para formar un polígono.", "Advertencia", MB_OK | MB_ICONWARNING);
            }
        }
    }

    // CREACION DE MASCARAS PARA AJUSTAR TAMAÑO DE VIDEOS A LAS ZONAS DE LA PLANTILLA
    Mat mascaraCam = Mat::zeros(imagenOriginal.size(), CV_8UC1);
    vector <Point> ptsCamPoly;
    for (auto& p : puntosPoligonoCam)
        ptsCamPoly.push_back(Point(p.x, p.y));
    fillPoly(mascaraCam, vector <vector<Point>>{ptsCamPoly}, Scalar(255));

    Mat mascaraVid = Mat::zeros(imagenOriginal.size(), CV_8UC1);
    vector <Point> ptsVidPoly;
    for (auto& p : puntosPoligonoVid)
        ptsVidPoly.push_back(Point(p.x, p.y));
    fillPoly(mascaraVid, vector <vector<Point>>{ptsVidPoly}, Scalar(255));

    Rect bboxCam = boundingRect(puntosPoligonoCam);
    Rect bboxVid = boundingRect(puntosPoligonoVid);

    Mat frameCam, frameVid;
    Mat videoOverlayCam, videoOverlayVid; // FRAMES DE VIDEOS PARA SUPERPOSICION

    // DEFINICION DE UN KERNEL PARA APLICAR OPERACION MORFOLOGICA
    // LA OPERACION ES APERTURA, CON UN KERNEL DE 3X3
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));

    while (true){
        capCam >> frameCam;
        capVid >> frameVid;

        if (frameCam.empty()){
            cerr << "Error: No hay frame de cámara para capturar. Reiniciando cámara..." << endl;
            capCam.release();
            capCam.open(0);
            if (!capCam.isOpened()) {
                cerr << "Error: No se pudo reiniciar la cámara. Saliendo." << endl;
                break;
            }
            continue;
        }

        if (capVid.isOpened() && frameVid.empty()){ // CONDICIONAL PARA CUANDO EL VIDEO FINALICE SE REINICIE
            cerr << "Error: No hay frame de video para capturar. Reiniciando video..." << endl;
            capVid.release();
            capVid.open(rutaVideo);
            if (!capVid.isOpened()) {
                cerr << "Error: No se pudo reiniciar el video. Continuará sin video." << endl;
            }
            continue;
        }

        Mat imagenVideos = imagenOriginal.clone(); // Siempre clona la imagen original limpia

        // Procesar y superponer la cámara
        if (!frameCam.empty()) {
            resize(frameCam, videoOverlayCam, bboxCam.size());

            // Aplicar filtro ecualizacion Histograma a la cámara
            if (aplica_ecualizacion_cam) {
                Mat grayCam, eqCam;
                cvtColor(videoOverlayCam, grayCam, COLOR_BGR2GRAY);
                equalizeHist(grayCam, eqCam);
                cvtColor(eqCam, videoOverlayCam, COLOR_GRAY2BGR);
            }

            // Aplicar filtro CLAHE a la camara
            if (aplica_clahe_cam) {
                Mat grayCam, claheCam;
                cvtColor(videoOverlayCam, grayCam, COLOR_BGR2GRAY);
                clahe -> apply(grayCam, claheCam);
                cvtColor(claheCam, videoOverlayCam, COLOR_GRAY2BGR);
            }

            // Aplicar operacion de apertura a la camara
            if (aplica_morfologica_cam){
                Mat grayCam, aperturaCam;
                cvtColor(videoOverlayCam, grayCam, COLOR_BGR2GRAY);
                morphologyEx(grayCam, aperturaCam, MORPH_OPEN, kernel);
                cvtColor(aperturaCam, videoOverlayCam, COLOR_GRAY2BGR);

            }

            // Aplicar suavizado bilateral a la camara
            if (aplica_bilateral_cam){
                Mat bilateralCam;
                // Parametros definidos: diametro = 9, sigmaColor = 75, sigmaSpace = 75
                bilateralFilter(videoOverlayCam, bilateralCam, 9, 75, 75);
                videoOverlayCam = bilateralCam;
            }

            // Crear ROI para la superposición
            Mat roiCam = imagenVideos(bboxCam);
            Mat maskedCamVideo;
            videoOverlayCam.copyTo(maskedCamVideo, mascaraCam(bboxCam)); // Aplica la máscara sobre el video recortado
            maskedCamVideo.copyTo(roiCam, mascaraCam(bboxCam)); // Superponer solo donde la máscara es blanca
        }

        // Procesar y superponer el video (si está disponible)
        if (!frameVid.empty() && capVid.isOpened()) {
            resize(frameVid, videoOverlayVid, bboxVid.size());

            // Aplicacion de filtros al video
            if (aplica_ecualizacion_vid){
                Mat grayVid, eqVid;
                cvtColor(videoOverlayVid, grayVid, COLOR_BGR2GRAY);
                equalizeHist(grayVid, eqVid);
                cvtColor(eqVid, videoOverlayVid, COLOR_GRAY2BGR);
            }

            // Aplicar filtro CLAHE al video
            if (aplica_clahe_vid){
                Mat grayVid, claheVid;
                cvtColor(videoOverlayVid, grayVid, COLOR_BGR2GRAY);
                clahe -> apply(grayVid, claheVid);
                cvtColor(claheVid, videoOverlayVid, COLOR_GRAY2BGR);
            }

            // Aplicar operacion de apertura al video
            if (aplica_morfologica_vid){
                Mat grayVid, aperturaVid;
                cvtColor(videoOverlayVid, grayVid, COLOR_BGR2GRAY);
                morphologyEx(grayVid, aperturaVid, MORPH_OPEN, kernel);
                cvtColor(aperturaVid, videoOverlayVid, COLOR_GRAY2BGR);

            }

            // Aplicar suavizado bilateral a la camara
            if (aplica_bilateral_vid){
                Mat bilateralVid;
                // Parametros definidos: diametro = 9, sigmaColor = 75, sigmaSpace = 75
                bilateralFilter(videoOverlayVid, bilateralVid, 9, 75, 75);
                videoOverlayVid = bilateralVid;
            }

            // Crear ROI para la superposición
            Mat roiVid = imagenVideos(bboxVid);
            Mat maskedVidVideo;
            videoOverlayVid.copyTo(maskedVidVideo, mascaraVid(bboxVid));
            maskedVidVideo.copyTo(roiVid, mascaraVid(bboxVid));
        }

        imshow("Imagen con Videos", imagenVideos);

        // Control de teclado para filtros y salir
        int key = waitKey(1); // waitKey(1) para permitir alta tasa de frames
        if (key == 27) { // ESC para salir
            break;
        }
        if (key == 'e' || key == 'E') {
            aplica_ecualizacion_cam = !aplica_ecualizacion_cam;
            cout << "Ecualizacion de Camara: " << (aplica_ecualizacion_cam ? "ACTIVADA" : "DESACTIVADA") << endl;
        }
        if (key == 'd' || key == 'D') {
            aplica_ecualizacion_vid = !aplica_ecualizacion_vid;
            cout << "Ecualizacion del Video: " << (aplica_ecualizacion_vid ? "ACTIVADA" : "DESACTIVADA") << endl;
        }
        if (key == 'r' || key == 'R') {
            aplica_clahe_cam = !aplica_clahe_cam;
            cout << "CLAHE de Camara: " << (aplica_clahe_cam ? "ACTIVADA" : "DESACTIVADA") << endl;
        }
        if (key == 'f' || key == 'F'){
            aplica_clahe_vid = !aplica_clahe_vid;
            cout << "CLAHE de Video: " << (aplica_clahe_vid ? "ACTIVADA" : "DESACTIVADA") <<  endl;
        }
        if (key == 't' || key == 'T'){
            aplica_morfologica_cam = !aplica_morfologica_cam;
            cout << "APERTURA de Camara: " << (aplica_morfologica_cam ? "ACTIVADA" : "DESACTIVADA") << endl;
        }
        if (key == 'g' || key == 'G'){
            aplica_morfologica_vid = !aplica_morfologica_vid;
            cout << "APERTURA de Video: " << (aplica_morfologica_vid ? "ACTIVADA" : "DESACTIVADA") << endl;
        }
        if (key == 'y' || key == 'Y'){
            aplica_bilateral_cam = !aplica_bilateral_cam;
            cout << "SUAVIZADO BILATERAL DE CAMARA: " << (aplica_bilateral_cam ? "ACTIVADA" : "DESACTIVADA") << endl;
        }
        if (key == 'h' || key == 'H'){
            aplica_bilateral_vid = !aplica_bilateral_vid;
            cout << "SUAVIZADO BILATERAL DE VIDEO: " << (aplica_bilateral_vid ? "ACTIVADA" : "DESACTIVADA") << endl;
        }
        // Puedes añadir más controles para otros filtros aquí:
        // if (key == 'c' || key == 'C') { aplica_clahe = !aplica_clahe; ... }
        // if (key == 'm' || key == 'M') { aplica_morfologica = !aplica_morfologica; ... }
    }

    capCam.release();
    capVid.release();
    destroyAllWindows(); // Asegúrate de destruir todas las ventanas al final
    return 0;
}