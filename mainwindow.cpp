#include "mainwindow.h"
#include "./ui_mainwindow.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->stop_or_resume->setVisible(false);
    ui->quantam_label->setVisible(false);
    ui->quantam->setVisible(false);
    connect(ui->num, SIGNAL(textChanged(QString)), this, SLOT(updateTable()));
    connect(ui->Schedular_type, SIGNAL(currentIndexChanged(int)), this, SLOT(enableOrDisablePriority(int)));
    ui->readyQeue->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->inputTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    connect(ui->Schedular_type, &QComboBox::currentIndexChanged, this, &MainWindow::hideResumeButton);
    connect(ui->num, &QLineEdit::textChanged, this, &MainWindow::hideResumeButton);
    connect(ui->quantam, &QLineEdit::textChanged, this, &MainWindow::hideResumeButton);
    connect(ui->inputTable, &QTableWidget::itemChanged, this, &MainWindow::hideResumeButton);
    ui->readyQeue->setVisible(false);
    ui->readyQeue->verticalHeader()->setVisible(false);
    ui->readyQeue->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::drawNextRectangle);
    ui->waiting_time->setVisible(false);
    ui->turn_time->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProcesses(vector<string>&v)
{
    if(!done[m_currentIndex-1]){
        done[m_currentIndex-1]=true;
    }
    else
        return;










    if (pre_row >= 0 && pre_row < ui->readyQeue->rowCount()) {
        QTableWidgetItem* stateItem = ui->readyQeue->item(pre_row, 2);
        if (!(stateItem->text() == "finished")) {
            QTableWidgetItem* ready = new QTableWidgetItem("ready");
            ui->readyQeue->setItem(pre_row, 2, ready);
        }
    }














    string str = v[m_currentIndex-1];
    int index = 0;
    for (int i = 0; i < processes.size(); i++) {
        if (str == processes[i].processName && processes[i].burstTime > 0) {
            QString processName = QString::fromStdString("Process" + processes[i].processName);
            processes[i].burstTime = processes[i].burstTime - 1;
            int remain = processes[i].burstTime;

            QTableWidgetItem* remaining = new QTableWidgetItem(QString::number(remain));
            QTableWidgetItem* state = new QTableWidgetItem("running");

            int rowToUpdate = -1;
            for (int row = 0; row < ui->readyQeue->rowCount(); ++row) {
                if (ui->readyQeue->item(row, 0)->text() == processName) {
                    rowToUpdate = row;
                    break;
                }
            }

            if (rowToUpdate != -1) {

                ui->readyQeue->setItem(rowToUpdate, 1, remaining);
                ui->readyQeue->setItem(rowToUpdate, 2, state);
                pre_row=rowToUpdate;
            }

            if (processes[i].burstTime <= 0) {
                processes[i].burstTime = 0; // Ensure burst time is not negative
                state->setText("finished");
            }

            break; // Exit loop after updating the process
        }
    }
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int red = 248;
    int green = 249;
    int blue = 250;

    // Create a QColor object with the specified RGB values
    QColor greenColor(red, green, blue);
    painter.fillRect(rect(), greenColor);
    ui->time->setText(QString::number(m_currentIndex) + " seconds");
    int ul_start_x = 910;
    int ul_end_x = 1510;
    int rectWidth = 0;
    for (int i = 0; i < m_currentIndex; ++i) {
        if(m_currentIndex >= 60)
        {
            rectWidth = 10;
        }
        else
        {
            rectWidth = (ul_end_x - ul_start_x)/(m_currentIndex);
        }
        const auto &rectangle = m_rectangles[i];
        int x = ul_start_x + rectWidth * (i%60);
        int y = rectangle.startY;

        painter.setBrush(rectangle.color);
        painter.drawRect(x, y, rectWidth, rectangle.height);


        QFont font = painter.font();
        font.setPointSize(10);
        painter.setFont(font);
        QRect textRect = QRect(x, y, rectWidth, rectangle.height);

        painter.drawText(textRect, Qt::AlignCenter, rectangle.text);
    }

    if(m_currentIndex)
        updateProcesses(d.ganttChart);
}
void MainWindow::drawNextRectangle() {

    if (m_currentIndex < m_rectangles.size()) {
        m_currentIndex++;
        createreadyqeue();

        if (m_currentIndex == m_rectangles.size()) {


            double wait = data.avgWaitingTime;
            double turn = data.avgTurnAroundTime;
            if(wait<=1e-2){wait=0;}
            if(turn<=1e-2){turn=0;}
            ui->waiting_time->setVisible(true);
            ui->turn_time->setVisible(true);
            ui->waiting_time->setText(QString("Average Turnaround: ") + QString::number(turn,'f',2));
            ui->turn_time->setText(QString("Average Waiting: ") + QString::number(wait,'f',2));

        }

        update();
    } else {
        m_timer->stop();
    }
}

void MainWindow::updateTable()
{
    bool ok;
    int rowCount = ui->num->text().toInt(&ok);
    if (ok) {
        createTable(rowCount, 4);
    } else {
        ui->inputTable->setRowCount(0);
    }
}

void MainWindow::createTable(int rows, int cols)
{
    ui->inputTable->setRowCount(rows);
    ui->inputTable->setColumnCount(cols);
    ui->inputTable->verticalHeader()->setVisible(false);
    for (int row = 0; row < rows; ++row) {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(row+1));
        ui->inputTable->setItem(row, 0, item);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }

    for (int row = 0; row < rows; ++row) {


        for (int col = 1; col < cols; ++col) {

            QTableWidgetItem *item = new QTableWidgetItem(
                QString("X"));
            ui->inputTable->setItem(row, col, item);
        }
        ui->inputTable->item(row, 3)->setFlags(ui->inputTable->item(row, 3)->flags() & ~Qt::ItemIsEditable);

    }
}
void MainWindow::createreadyqeue()
{
    ui->readyQeue->setRowCount( processes.size() + 1);
    ui->readyQeue->setColumnCount(3);
    bool ok;
    for (int i = 0; i < processes.size(); i++) {
        if (m_currentIndex == processes[i].arrivalTime) {
            QString processName = QString::fromStdString("Process"+processes[i].processName);
            QTableWidgetItem* item = new QTableWidgetItem(processName);
            QTableWidgetItem *remaining = new QTableWidgetItem(QString::number(processes[i].burstTime));
            QTableWidgetItem* state = new QTableWidgetItem("Ready");
            ui->readyQeue->setItem(readyindex, 0, item);
            ui->readyQeue->setItem(readyindex, 1, remaining);
            ui->readyQeue->setItem(readyindex, 2, state);
            readyindex++;
        }
    }
}

void MainWindow::readTableData() {
    int rowCount = ui->inputTable->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* nameItem = ui->inputTable->item(row, 0);
        QTableWidgetItem* arrivalItem = ui->inputTable->item(row, 1);
        QTableWidgetItem* burstItem = ui->inputTable->item(row, 2);
        QTableWidgetItem* priorityItem = ui->inputTable->item(row, 3);

        if (nameItem && arrivalItem && burstItem && priorityItem) {
            std::string processName = nameItem->text().toStdString();
            int arrivalTime = arrivalItem->text().toInt();
            int burstTime = burstItem->text().toInt();
            int priority = priorityItem->text().toInt();

            Process process(processName, arrivalTime, burstTime, priority);
            processes.push_back(process);

        }
    }
}

void MainWindow::hideResumeButton()
{
    if(ui->Schedular_type->currentIndex()==5)
    {
        ui->quantam_label->setVisible(true);
        ui->quantam->setVisible(true);
    }
    else
    {
        ui->quantam_label->setVisible(false);
        ui->quantam->setVisible(false);
    }

    m_timer->stop();
    ui->time->setText("0 seconds");
    ui->stop_or_resume->setVisible(false);
    ui->stop_or_resume->setText("Stop");
}



void MainWindow::enableOrDisablePriority(int index)
{
    if(index==0 || index == 1 || index == 2 || index == 5)
    {   for (int row = 0; row < ui->inputTable->rowCount(); ++row)
            ui->inputTable->item(row, 3)->setFlags(ui->inputTable->item(row, 3)->flags() & ~Qt::ItemIsEditable);
    }
    else
    {
        for (int row = 0; row < ui->inputTable->rowCount(); ++row)
            ui->inputTable->item(row, 3)->setFlags(ui->inputTable->item(row, 3)->flags() | Qt::ItemIsEditable);
    }
}

void MainWindow::AddRectangles(vector<string>&v)
{
    int color_idx = 0;
    RectangleProperties rectangle;
    int x = 910;
    int y = 30;
    for(int i = 0;i<v.size();i++)
    {
        if(i%60==0)
        {
            y += 100;
            x = 910;
        }
        QColor color;
        QString key = QString::fromStdString(v[i]);
        if (colorMap.count(key) > 0) {
            color = colorMap[key];
        } else {
            colorMap[key] = lightColors[color_idx];
            color = colorMap[key];
            color_idx++;
            color_idx%=25;
        }
        rectangle = {color, key,y,100};
        m_rectangles.push_back(rectangle);
        x+=10;
    }
}


void MainWindow::on_dynamicbutt_clicked()
{
    done.clear();
    ui->waiting_time->setVisible(false);
    ui->turn_time->setVisible(false);
    ui->time->setText("0 seconds");
    ui->stop_or_resume->setVisible(true);
    ui->readyQeue->setVisible(true);

    readyindex = 0;
    m_currentIndex=0;
    pre_row=-1;
    m_rectangles.clear();
    colorMap.clear();
    processes.clear();
    readTableData();
    int index = ui->Schedular_type->currentIndex();
    if(index==0)
        d = fcfs(processes);
    else if (index == 1)
        d = SJF_Preemptive(processes);
    else if (index == 2)
        d = SJF_NonPreemptive(processes);
    else if(index == 3)
        d = preemptive_priority(processes);
    else if(index == 4)
        d = non_preemptive_priority(processes);
    else
        d = RR(processes,ui->quantam->text().toInt());
    data=d;
    done.resize(d.ganttChart.size());
    AddRectangles(d.ganttChart);
    ui->readyQeue->clearContents();
    ui->readyQeue->setRowCount(0);
    createreadyqeue();
    m_timer->start(1000);
}



void MainWindow::on_stop_or_resume_clicked()
{
    if(ui->stop_or_resume->text() == "Stop")
    {
        m_timer->stop();
        ui->stop_or_resume->setText("Resume");
    }
    else
    {
        m_timer->start(1000);
        ui->stop_or_resume->setText("Stop");
    }
}


void MainWindow::on_staticbutt_clicked()
{
    ui->readyQeue->setVisible(false);
    m_rectangles.clear();
    colorMap.clear();
    processes.clear();
    readTableData();
    int index = ui->Schedular_type->currentIndex();

    if(index==0)
       d = fcfs(processes);
    else if (index == 1)
        d = SJF_Preemptive (processes);
    else if (index == 2)
        d = SJF_NonPreemptive(processes);
    else if(index == 3)
        d = preemptive_priority(processes);
    else if(index == 4)
        d = non_preemptive_priority(processes);
    else
        d = RR(processes,ui->quantam->text().toInt());
    data=d;

    done.clear();
    done.resize(d.ganttChart.size());
    AddRectangles(d.ganttChart);

    double wait = data.avgWaitingTime;
    double turn = data.avgTurnAroundTime;
    if(wait<=1e-2){wait=0;}
     if(turn<=1e-2){turn=0;}
    ui->waiting_time->setVisible(true);
    ui->turn_time->setVisible(true);
    ui->waiting_time->setText(QString("Average Turnaround: ") + QString::number(turn,'f',2));
    ui->turn_time->setText(QString("Average Waiting: ") + QString::number(wait,'f',2));
    m_currentIndex = m_rectangles.size();
    update();
}
