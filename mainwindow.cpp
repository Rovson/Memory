#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <random>
#include <algorithm>
#include <QUrl>
#include <QSoundEffect>
using namespace std;

// Constructor de la clase MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , firstButton(nullptr)
    , secondButton(nullptr)
{
    ui->setupUi(this);

    // Inicializar los sonidos
    openSound.setSource(QUrl::fromLocalFile(":/sonidos/open.wav"));
    doneSound.setSource(QUrl::fromLocalFile(":/sonidos/done.wav"));
    closeSound.setSource(QUrl::fromLocalFile(":/sonidos/close.wav"));
    errorSound.setSource(QUrl::fromLocalFile(":/sonidos/error.wav"));
    winSound.setSource(QUrl::fromLocalFile(":/sonidos/win.wav"));

    on_actionreiniciar_triggered();
}

// Destructor de la clase MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}

// Slot para salir de la aplicación
void MainWindow::on_actionsalir_triggered()
{
    qApp->exit(0);
}

// Slot para mostrar información "Acerca de"
void MainWindow::on_actionAcercade_triggered()
{
    QMessageBox::about(this, "Acerca de", "Programa para mejorar la memoria, creado por Carlos Rovira López");
}

// Slot para reiniciar el juego
void MainWindow::on_actionreiniciar_triggered()
{
    firstButton = nullptr;
    secondButton = nullptr;

    // Inicializar la matriz de botones
    B[0][0] = ui->m00;
    B[0][1] = ui->m01;
    B[0][2] = ui->m02;
    B[0][3] = ui->m03;
    B[0][4] = ui->m04;
    B[0][5] = ui->m05;
    B[1][0] = ui->m10;
    B[1][1] = ui->m11;
    B[1][2] = ui->m12;
    B[1][3] = ui->m13;
    B[1][4] = ui->m14;
    B[1][5] = ui->m15;
    B[2][0] = ui->m20;
    B[2][1] = ui->m21;
    B[2][2] = ui->m22;
    B[2][3] = ui->m23;
    B[2][4] = ui->m24;
    B[2][5] = ui->m25;
    B[3][0] = ui->m30;
    B[3][1] = ui->m31;
    B[3][2] = ui->m32;
    B[3][3] = ui->m33;
    B[3][4] = ui->m34;
    B[3][5] = ui->m35;
    B[4][0] = ui->m40;
    B[4][1] = ui->m41;
    B[4][2] = ui->m42;
    B[4][3] = ui->m43;
    B[4][4] = ui->m44;
    B[4][5] = ui->m45;

    // Generar y barajar los iconos
    QVector<QString> iconPaths;
    for (int i = 0; i < 15; ++i) {
        QString iconPath = ":/Iconos/ufo" + QString("%1").arg(i, 2, 10, QChar('0')) + ".png";
        iconPaths.append(iconPath);
        iconPaths.append(iconPath);
    }

    shuffleIcons(iconPaths); // Barajar los iconos

    // Asignar iconos a los botones
    int index = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            icons[i][j] = iconPaths[index++];
            B[i][j]->setIcon(QIcon(":/Iconos/topsecret.png"));
            B[i][j]->setEnabled(true);
            B[i][j]->setProperty("row", i);
            B[i][j]->setProperty("col", j);
            B[i][j]->setProperty("matched", false); // Propiedad para marcar coincidencias
        }
    }
}

// Barajar los iconos usando un generador aleatorio
void MainWindow::shuffleIcons(QVector<QString> &iconPaths)
{
    random_device rd;
    mt19937 g(rd());
    shuffle(iconPaths.begin(), iconPaths.end(), g);
}

// Lógica para manejar el clic de un botón
void MainWindow::Pulsar(int pi, int pj)
{
    if (B[pi][pj]->property("matched").toBool()) {
        return; // Si el botón ya ha sido coincidente, no hacer nada
    }

    if (!firstButton) {
        firstButton = B[pi][pj];
        firstButton->setIcon(QIcon(icons[pi][pj]));
        openSound.play(); // Sonido al descubrir una casilla
    } else if (firstButton == B[pi][pj]) {
        return; // Si el usuario hizo clic en el mismo botón dos veces, no hacer nada
    } else if (!secondButton) {
        secondButton = B[pi][pj];
        secondButton->setIcon(QIcon(icons[pi][pj]));
        openSound.play(); // Sonido al descubrir una casilla
        QTimer::singleShot(1000, this, &MainWindow::checkForMatch);
    } else {
        errorSound.play(); // Sonido al intentar descubrir una tercera casilla
    }
}

// Comprobar si las dos casillas seleccionadas coinciden
void MainWindow::checkForMatch()
{
    if (firstButton && secondButton) {
        int firstRow = firstButton->property("row").toInt();
        int firstCol = firstButton->property("col").toInt();
        int secondRow = secondButton->property("row").toInt();
        int secondCol = secondButton->property("col").toInt();

        if (icons[firstRow][firstCol] == icons[secondRow][secondCol]) {
            QString matchedIconPath = icons[firstRow][firstCol];
            QString matchedIconName = matchedIconPath.mid(matchedIconPath.lastIndexOf("/") + 1, matchedIconPath.lastIndexOf(".") - matchedIconPath.lastIndexOf("/") - 1);
            QString matchedIconCPath = QString(":/Iconos/%1c.png").arg(matchedIconName);

            firstButton->setIcon(QIcon(matchedIconCPath));
            secondButton->setIcon(QIcon(matchedIconCPath));

            // Marcar los botones como coincidencia encontrada
            firstButton->setProperty("matched", true);
            secondButton->setProperty("matched", true);
            doneSound.play(); // Sonido al emparejar una casilla

            checkIfGameWon(); // Verificar si el juego ha sido ganado
        } else {
            closeSound.play(); // Sonido al ocultar una casilla
            QTimer::singleShot(500, this, [this]() {
                if (firstButton) {
                    firstButton->setIcon(QIcon(":/Iconos/topsecret.png"));
                }
                if (secondButton) {
                    secondButton->setIcon(QIcon(":/Iconos/topsecret.png"));
                }
                firstButton = nullptr;
                secondButton = nullptr;
            });
            return; // Salir de la función antes de resetear los botones
        }

        firstButton = nullptr;
        secondButton = nullptr;
    }
}

// Verificar si todas las parejas han sido encontradas
void MainWindow::checkIfGameWon()
{
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (!B[i][j]->property("matched").toBool()) {
                return; // Si encontramos un botón no coincidente, no hacer nada
            }
        }
    }

    // Si todos los botones coinciden, mostrar mensaje de victoria y sonido
    winSound.play();

    QMessageBox msgBox;
    msgBox.setWindowTitle("¡Felicidades!");
    msgBox.setText("Has ganado.Tienes buena memoria ¿Reiniciar o salir?");
    msgBox.setIconPixmap(QPixmap(":/Iconos/icon.png"));
    msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);

    int reply = msgBox.exec();
    if (reply == QMessageBox::Retry) {
        on_actionreiniciar_triggered();
    } else {
        qApp->exit(0);
    }
}

// --------------------------
// CLICK DE TODOS LOS BOTONES
// --------------------------

void MainWindow::on_m00_clicked() { Pulsar(0, 0); }
void MainWindow::on_m01_clicked() { Pulsar(0, 1); }
void MainWindow::on_m02_clicked() { Pulsar(0, 2); }
void MainWindow::on_m03_clicked() { Pulsar(0, 3); }
void MainWindow::on_m04_clicked() { Pulsar(0, 4); }
void MainWindow::on_m05_clicked() { Pulsar(0, 5); }
void MainWindow::on_m10_clicked() { Pulsar(1, 0); }
void MainWindow::on_m11_clicked() { Pulsar(1, 1); }
void MainWindow::on_m12_clicked() { Pulsar(1, 2); }
void MainWindow::on_m13_clicked() { Pulsar(1, 3); }
void MainWindow::on_m14_clicked() { Pulsar(1, 4); }
void MainWindow::on_m15_clicked() { Pulsar(1, 5); }
void MainWindow::on_m20_clicked() { Pulsar(2, 0); }
void MainWindow::on_m21_clicked() { Pulsar(2, 1); }
void MainWindow::on_m22_clicked() { Pulsar(2, 2); }
void MainWindow::on_m23_clicked() { Pulsar(2, 3); }
void MainWindow::on_m24_clicked() { Pulsar(2, 4); }
void MainWindow::on_m25_clicked() { Pulsar(2, 5); }
void MainWindow::on_m30_clicked() { Pulsar(3, 0); }
void MainWindow::on_m31_clicked() { Pulsar(3, 1); }
void MainWindow::on_m32_clicked() { Pulsar(3, 2); }
void MainWindow::on_m33_clicked() { Pulsar(3, 3); }
void MainWindow::on_m34_clicked() { Pulsar(3, 4); }
void MainWindow::on_m35_clicked() { Pulsar(3, 5); }
void MainWindow::on_m40_clicked() { Pulsar(4, 0); }
void MainWindow::on_m41_clicked() { Pulsar(4, 1); }
void MainWindow::on_m42_clicked() { Pulsar(4, 2); }
void MainWindow::on_m43_clicked() { Pulsar(4, 3); }
void MainWindow::on_m44_clicked() { Pulsar(4, 4); }
void MainWindow::on_m45_clicked() { Pulsar(4, 5); }
