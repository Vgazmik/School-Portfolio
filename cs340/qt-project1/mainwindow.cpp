#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
  * Print out for first push button...
  */
void MainWindow::on_pushButton_clicked()
{
    std::cout << "Saving World... this may take a few hours... " << std::endl;
}

/**
  * Print out for second push button...
  */
void MainWindow::on_pushButton_2_clicked()
{
    std::cout << "Destroying World... this may take a few hours..." << std::endl;
}

/**
  * Print out for first radio button...
  */
void MainWindow::on_radioButton_toggled(bool checked)
{
    if (checked)
        std::cout << "~Toggled~" << std::endl;
    else
        std::cout << "~Untoggled~" << std::endl;
}
