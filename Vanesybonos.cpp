#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QMessageBox>
#include <QLocale>
#include <algorithm>

// Constantes de la Ley Organica del Trabajo (Venezuela)
#define DIAS_BONO_VACACIONAL_BASE 15
#define DIAS_VACACIONES_BASE 15
#define MAX_DIAS_ADICIONALES 15

// Funciones de cálculo lógico (Intactas)
double CalcularSalarioDiario(double salarioMensual) { return salarioMensual / 30.0; }

double CalcularBonoVacacional(int anosServicio, double salarioMensual) {
    int diasBono = DIAS_BONO_VACACIONAL_BASE + std::min(anosServicio, MAX_DIAS_ADICIONALES);
    return diasBono * CalcularSalarioDiario(salarioMensual);
}

double CalcularAntiguedad(int anosServicio, double salarioMensual) {
    int mesesServicio = anosServicio * 12;
    int mesesConAntiguedad = std::max(0, mesesServicio - 12);
    double antiguedadPorMes = 5 * CalcularSalarioDiario(salarioMensual);
    return mesesConAntiguedad * antiguedadPorMes;
}

double CalcularBonoProduccion(double salarioMensual, int horasTrabajadas) {
    if (horasTrabajadas > 160) return salarioMensual * 0.10;
    if (horasTrabajadas > 140) return salarioMensual * 0.05;
    return 0;
}

double CalcularBonoAdicional(int anosServicio, int horasTrabajadas) {
    double bono = 0;
    if (anosServicio >= 5) bono += 500000;
    else if (anosServicio >= 2) bono += 200000;
    
    if (horasTrabajadas > 200) bono += 300000;
    else if (horasTrabajadas > 180) bono += 150000;
    return bono;
}

int CalcularDiasVacaciones(int anosServicio) {
    return DIAS_VACACIONES_BASE + std::min(anosServicio, MAX_DIAS_ADICIONALES);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion"); // Base limpia para los estilos

    QWidget window;
    window.setWindowTitle("Sistema de Cálculo de Beneficios Laborales - Venezuela");
    window.resize(850, 750);

    // ==========================================
    // ESTILOS CSS (QSS) PARA ASPECTO PROFESIONAL
    // ==========================================
    window.setStyleSheet(R"(
        QWidget { background-color: #f4f6f9; }
        QLabel { font-family: 'Segoe UI'; font-size: 14px; color: #000000ff; }
        QLabel[title="true"] { font-weight: bold; font-size: 16px; color: #000000ff; border-bottom: 2px solid #3498db; padding-bottom: 5px; margin-top: 15px; }
        QLineEdit { border: 1px solid #000000ff; border-radius: 6px; padding: 6px; font-family: 'Segoe UI'; font-size: 14px; background-color: white; }
        QLineEdit:focus { border: 2px solid #3498db; }
        QPushButton { border-radius: 8px; padding: 10px 20px; font-family: 'Segoe UI'; font-size: 14px; font-weight: bold; color: white; }
        QPushButton#btnCalcular { background-color: #2980b9; border: 1px solid #2471a3; }
        QPushButton#btnCalcular:hover { background-color: #3498db; }
        QPushButton#btnLimpiar { background-color: #7f8c8d; border: 1px solid #707b7c; }
        QPushButton#btnLimpiar:hover { background-color: #95a5a6; }
        QTextEdit { font-family: 'Segoe UI'; font-size: 14px; border: 1px solid #bdc3c7; border-radius: 8px; padding: 15px; background-color: white; }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(20);

    // ==========================================
    // CREACIÓN DE CONTROLES
    // ==========================================
    auto createTitle = [](const QString &text) {
        QLabel *label = new QLabel(text);
        label->setProperty("title", true);
        label->setAlignment(Qt::AlignCenter);
        return label;
    };

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);

    // Fila 1: Datos Trabajador
    QLineEdit *txtNombre = new QLineEdit();
    QLineEdit *txtApellido = new QLineEdit();
    gridLayout->addWidget(createTitle("DATOS DEL TRABAJADOR"), 0, 0, 1, 4);
    gridLayout->addWidget(new QLabel("Nombre:"), 1, 0);
    gridLayout->addWidget(txtNombre, 1, 1);
    gridLayout->addWidget(new QLabel("Apellido:"), 1, 2);
    gridLayout->addWidget(txtApellido, 1, 3);

    // Fila 2: Ingreso y Salario
    QLineEdit *txtFechaIngreso = new QLineEdit("01/01/2020");
    txtFechaIngreso->setPlaceholderText("DD/MM/AAAA");
    QLineEdit *txtSalarioMensual = new QLineEdit("1000000");
    gridLayout->addWidget(new QLabel("Fecha de Ingreso:"), 2, 0);
    gridLayout->addWidget(txtFechaIngreso, 2, 1);
    gridLayout->addWidget(new QLabel("Salario Mensual (Bs.):"), 2, 2);
    gridLayout->addWidget(txtSalarioMensual, 2, 3);

    // Fila 3: Vacaciones
    QLineEdit *txtInicioVac = new QLineEdit("01/12/2024");
    QLineEdit *txtFinVac = new QLineEdit("15/12/2024");
    gridLayout->addWidget(createTitle("PERÍODO DE VACACIONES"), 3, 0, 1, 4);
    gridLayout->addWidget(new QLabel("Fecha Inicio (DD/MM/AAAA):"), 4, 0);
    gridLayout->addWidget(txtInicioVac, 4, 1);
    gridLayout->addWidget(new QLabel("Fecha Fin (DD/MM/AAAA):"), 4, 2);
    gridLayout->addWidget(txtFinVac, 4, 3);

    // Fila 4: Horas
    QLineEdit *txtHoras = new QLineEdit("160");
    gridLayout->addWidget(createTitle("HORAS TRABAJADAS"), 5, 0, 1, 4);
    gridLayout->addWidget(new QLabel("Horas trabajadas en el mes:"), 6, 0);
    gridLayout->addWidget(txtHoras, 6, 1);

    mainLayout->addLayout(gridLayout);

    // Botones
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnCalcular = new QPushButton("Calcular Beneficios");
    btnCalcular->setObjectName("btnCalcular");
    QPushButton *btnLimpiar = new QPushButton("Limpiar");
    btnLimpiar->setObjectName("btnLimpiar");
    btnLayout->addStretch();
    btnLayout->addWidget(btnCalcular, 2);
    btnLayout->addWidget(btnLimpiar, 1);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    // Resultados
    mainLayout->addWidget(createTitle("RESULTADOS DEL CÁLCULO"));
    QTextEdit *txtResultado = new QTextEdit();
    txtResultado->setReadOnly(true);
    mainLayout->addWidget(txtResultado);

    // ==========================================
    // LÓGICA DE EVENTOS (SEÑALES Y SLOTS)
    // ==========================================
    QObject::connect(btnLimpiar, &QPushButton::clicked, [=]() {
        txtNombre->clear();
        txtApellido->clear();
        txtFechaIngreso->setText("01/01/2020");
        txtInicioVac->setText("01/12/2024");
        txtFinVac->setText("15/12/2024");
        txtHoras->setText("160");
        txtSalarioMensual->setText("1000000");
        txtResultado->clear();
    });

    QObject::connect(btnCalcular, &QPushButton::clicked, [=]() {
        if (txtNombre->text().isEmpty() || txtApellido->text().isEmpty()) {
            QMessageBox::critical(nullptr, "Error", "Por favor ingrese nombre y apellido.");
            return;
        }

        double salarioMensual = txtSalarioMensual->text().toDouble();
        int horasTrabajadas = txtHoras->text().toInt();

        if (salarioMensual <= 0) {
            QMessageBox::critical(nullptr, "Error", "Ingrese un salario válido.");
            return;
        }

        QString format = "dd/MM/yyyy";
        QDate fechaIngreso = QDate::fromString(txtFechaIngreso->text(), format);
        QDate inicioVac = QDate::fromString(txtInicioVac->text(), format);
        QDate finVac = QDate::fromString(txtFinVac->text(), format);

        if (!fechaIngreso.isValid() || !inicioVac.isValid() || !finVac.isValid()) {
            QMessageBox::critical(nullptr, "Error", "Formato de fecha inválido. Use DD/MM/AAAA.");
            return;
        }

        if (inicioVac > finVac) {
            QMessageBox::critical(nullptr, "Error", "La fecha de inicio debe ser anterior a la de fin.");
            return;
        }

        // Cálculo de años de servicio con QDate (muy preciso)
        int anosServicio = fechaIngreso.daysTo(QDate::currentDate()) / 365.25;

        // Cálculo de días hábiles entre dos fechas
        int diasHabilesVac = 0;
        QDate tempDate = inicioVac;
        while (tempDate <= finVac) {
            if (tempDate.dayOfWeek() >= 1 && tempDate.dayOfWeek() <= 5) { // 1=Lunes, 5=Viernes
                diasHabilesVac++;
            }
            tempDate = tempDate.addDays(1);
        }

        double bonoVacacional = CalcularBonoVacacional(anosServicio, salarioMensual);
        double antiguedad = CalcularAntiguedad(anosServicio, salarioMensual);
        double bonoProduccion = CalcularBonoProduccion(salarioMensual, horasTrabajadas);
        double bonoAdicional = CalcularBonoAdicional(anosServicio, horasTrabajadas);
        int diasVacaciones = CalcularDiasVacaciones(anosServicio);
        double totalBeneficios = bonoVacacional + antiguedad + bonoProduccion + bonoAdicional;

        // Formateador de moneda de Qt
        QLocale locale(QLocale::Spanish, QLocale::Venezuela);

        QString resultado = "<b>DATOS DEL TRABAJADOR:</b><br>";
        resultado += "Nombre completo: " + txtNombre->text() + " " + txtApellido->text() + "<br>";
        resultado += "Fecha de ingreso: " + fechaIngreso.toString(format) + "<br>";
        resultado += "Años de servicio: " + QString::number(anosServicio) + " años<br>";
        resultado += "Salario mensual: Bs. " + locale.toString(salarioMensual, 'f', 2) + "<br><br>";

        resultado += "<b>BONIFICACIONES Y BENEFICIOS:</b><br>";
        resultado += "Bono Vacacional (Art. 190 LOTTT): Bs. " + locale.toString(bonoVacacional, 'f', 2) + "<br>";
        resultado += "Prestación de Antigüedad (Art. 108 LOTTT): Bs. " + locale.toString(antiguedad, 'f', 2) + "<br>";
        resultado += "Bono de Producción: Bs. " + locale.toString(bonoProduccion, 'f', 2) + "<br>";
        resultado += "Bono Adicional por Antigüedad: Bs. " + locale.toString(bonoAdicional, 'f', 2) + "<br><br>";

        resultado += "<b>VACACIONES:</b><br>";
        resultado += "Días de vacaciones según ley (Art. 191 LOTTT): " + QString::number(diasVacaciones) + " días<br>";
        resultado += "Período solicitado: " + inicioVac.toString(format) + " al " + finVac.toString(format) + "<br>";
        resultado += "Días hábiles en el período: " + QString::number(diasHabilesVac) + " días<br><br>";

        resultado += "<b><font color='#27ae60' size='4'>TOTAL BENEFICIOS A RECIBIR: Bs. " + locale.toString(totalBeneficios, 'f', 2) + "</font></b>";

        // Mostrar texto justificado y renderizado con HTML
        txtResultado->setHtml("<div align='justify'>" + resultado + "</div>");
    });

    window.show();
    return app.exec();
}
