#include "showdialog.h"
#include "ui_showdialog.h"

void showDialog::initShm()
{
    // 创建并初始化有名信号灯 读
    semr = sem_open("mysem_r", O_CREAT | O_RDWR , 0666, 0);
    if (semr == SEM_FAILED)
    {
            perror("QT initShm sem_open:");
            exit(-1);
    }
    // 创建并初始化有名信号灯 写
    semw = sem_open("mysem_w", O_CREAT | O_RDWR, 0666, 1);
    if (semw == SEM_FAILED)
    {
            perror("QT initShm sem_open:");
            exit(-1);
    }
    int shmid_def;

    // 创建共享内存
    shmid_def = shmget((key_t)1236, sizeof(char)*SHAREMEM_LEN, 0666|IPC_CREAT);
    printf("Qt  shmCom  shmid_def:  %d-%d\n",shmid_def,sizeof(char)*SHAREMEM_LEN);
    if (shmid_def == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    // 获得共享存储所占用的实际地址
    if ((shmAddress = (char *)shmat(shmid_def, NULL, 0)) == (char *)(-1))
    {
            perror("shmat:");
            exit(-1);
    }
}
void showDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
    default:
            break;
    }
}

showDialog::showDialog(int screenWidth,int screenHeight,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showDialog)
{
    ui->setupUi(this);
    this->setMinimumSize(QSize(screenWidth, screenHeight));
    this->setMaximumSize(QSize(screenWidth, screenHeight));
    this->ui->tabWidget->setMinimumSize(QSize(screenWidth, screenHeight));
    this->ui->tabWidget->setMaximumSize(QSize(screenWidth, screenHeight));

    initShm();
    ScreenWidth = screenWidth;
    ScreenHeight = screenHeight;
    this->ui->tabWidget->clear();

    qDebug() << "load screen width,height:"<<ScreenWidth << "--" << ScreenHeight;
    m_pFormSale700 = new Formsale700(this);
    m_pFormSale700->setFixedSize(ScreenWidth, ScreenHeight);   // 设置窗体大小

    m_pFormLogin700 = new LoginForm700(this);
    m_pFormLogin700->setFixedSize(ScreenWidth, ScreenHeight);   // 设置窗体大小

    m_pFormPay700 = new Formpay700(this);
    m_pFormPay700->setFixedSize(ScreenWidth, ScreenHeight);   // 设置窗体大小

    m_pFormMenu700 = new Formmenu700(this);
    m_pFormMenu700->setFixedSize(ScreenWidth, ScreenHeight);   // 设置窗体大小

    m_pFormCalibration700 = new FormCalibration700(this);
    m_pFormCalibration700->setFixedSize(ScreenWidth, ScreenHeight);   // 设置窗体大小

    m_pFormReport700 = new FormReport700(this);
    m_pFormReport700->setFixedSize(ScreenWidth, ScreenHeight);   // 设置窗体大小

    m_pFormGoodsKey700 = new FormGoodsKey700(this);
    m_pFormGoodsKey700->setFixedSize(ScreenWidth, ScreenHeight);   // 设置窗体大小

    this->ui->tabWidget->insertTab(0, m_pFormLogin700, "login");
    this->ui->tabWidget->insertTab(1, m_pFormSale700, "sale");
    this->ui->tabWidget->insertTab(2, m_pFormPay700, "pay");
    this->ui->tabWidget->insertTab(3, m_pFormMenu700, "menu");
    this->ui->tabWidget->insertTab(4, m_pFormCalibration700, "cal");
    this->ui->tabWidget->insertTab(5, m_pFormReport700, "report");
    this->ui->tabWidget->insertTab(6, m_pFormGoodsKey700, "goodskey");

    m_pInputDialog700 = new inputDialog700(this);
    m_pInputDialog700->hide();

    m_pMsgDialog700 = new msgDialog700(this);
    m_pMsgDialog700->hide();

    m_PAddSaleDialog = new AddSaleDialog(this);
    m_PAddSaleDialog->hide();

    this->ui->tabWidget->setCurrentIndex(1);
    this->ui->tabWidget->findChildren<QTabBar*>().at(0)->hide();    // 去掉tab的标题

    QTimer *timerUpdate = new QTimer(this);
    timerUpdate->start(50); // 每次发射timeout信号时间间隔为50毫秒
    connect(timerUpdate, SIGNAL(timeout()), this, SLOT(DataUpdate()));

    qDebug() << "Qt screen load complete,inter timer update";
}

showDialog::~showDialog()
{
    delete ui;
}

void showDialog::DataUpdate()
{
    sem_wait(semr);
    stShowQT* pShowQT = (stShowQT*)shmAddress;
    if(false == pShowQT->update)
    {
        return;
    }
    pShowQT->update = false;
    //printf("pShowQT->type:%d\n",pShowQT->type);

    switch(pShowQT->type)
    {
         case enQt_LoginQT:
            m_pFormLogin700->updateLoginUi(&pShowQT->loginUi);
            this->ui->tabWidget->setCurrentIndex(0);
            break;
         case enQt_SaleQT:
            m_pFormSale700->updateSaleUi(&pShowQT->saleUi);
            this->ui->tabWidget->setCurrentIndex(1);
            break;
         case enQt_PayQT:
            m_pFormPay700->updatePayUi(&pShowQT->payUi);
            this->ui->tabWidget->setCurrentIndex(2);
            break;
         case enQt_MenuQT:
            m_pFormMenu700->updateMenuUi(&pShowQT->menuUi);
            this->ui->tabWidget->setCurrentIndex(3);
            break;
         case enQt_CalQt:
            m_pFormCalibration700->updateCalibrationUi(&pShowQT->CalibrationUi);
            this->ui->tabWidget->setCurrentIndex(4);
            break;
         case enQt_ReportQt:
            m_pFormReport700->updateReportUi(&pShowQT->ReportUi);
            this->ui->tabWidget->setCurrentIndex(5);
            break;
        case enQt_GoodsKeyQT:
            m_pFormGoodsKey700->updateGoodsKeyUi(&pShowQT->goodsKeyUi);
            this->ui->tabWidget->setCurrentIndex(6);
            break;
        case enQt_MsgQT:
            m_pMsgDialog700->updateMsgUi(&pShowQT->msgUi);
            break;
        case enQt_InputQT:
            m_pInputDialog700->updateInputMsgUi(&pShowQT->inputUi);
            break;
        case enQt_AddSaleQT:
            m_PAddSaleDialog->updateAddSaleUi(&pShowQT->addSaleUi);
            break;
        default:
            break;
    }
    sem_post(semw);
}


