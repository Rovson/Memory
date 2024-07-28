#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSoundEffect> // para los sonidos

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_m00_clicked();
    void on_m01_clicked();
    void on_m02_clicked();
    void on_m03_clicked();
    void on_m04_clicked();
    void on_m05_clicked();
    void on_m10_clicked();
    void on_m11_clicked();
    void on_m12_clicked();
    void on_m13_clicked();
    void on_m14_clicked();
    void on_m15_clicked();
    void on_m20_clicked();
    void on_m21_clicked();
    void on_m22_clicked();
    void on_m23_clicked();
    void on_m24_clicked();
    void on_m25_clicked();
    void on_m30_clicked();
    void on_m31_clicked();
    void on_m32_clicked();
    void on_m33_clicked();
    void on_m34_clicked();
    void on_m35_clicked();
    void on_m40_clicked();
    void on_m41_clicked();
    void on_m42_clicked();
    void on_m43_clicked();
    void on_m44_clicked();
    void on_m45_clicked();
    void on_actionreiniciar_triggered();
    void on_actionsalir_triggered();
    void on_actionAcercade_triggered();

private:
    Ui::MainWindow *ui;
    QPushButton *B[5][6]; // Matriz de botones para manejarlos
    QString icons[5][6]; // Matriz de nombres de iconos para compararlos
    QPushButton* firstButton; // Almacena el primer botón pulsado
    QPushButton* secondButton; // Almacena el segundo botón pulsado
    //Sonidos
    QSoundEffect openSound;
    QSoundEffect doneSound;
    QSoundEffect closeSound;
    QSoundEffect errorSound;
    QSoundEffect winSound;

    void Pulsar(int pi, int pj);
    void checkForMatch();
    void shuffleIcons(QVector<QString> &iconPaths);
    void checkIfGameWon();
};
#endif // MAINWINDOW_H


