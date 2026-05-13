 #pragma once
#include <msclr/marshal_cppstd.h>
#include "DatabaseManager.h"
#include "SportRepository.h"
#include "TournamentRepository.h"
#include "TeamRepository.h"
#include "PlayerRepository.h"
#include "MatchRepository.h"
#include "EncodingUtils.h"
#include <iostream>
#include <algorithm>  

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::ComponentModel;

// ========== ШРИФТ С ПОДДЕРЖКОЙ ЭМОДЗИ ==========
System::Drawing::Font^ GetEmojiFont(float size, FontStyle style)
{
    array<String^>^ emojiFonts = gcnew array<String^>{
        "Segoe UI Emoji", "Segoe UI", "Arial Unicode MS", "Microsoft Sans Serif"
    };

    for each (String ^ fontName in emojiFonts)
    {
        try
        {
            System::Drawing::Font^ font = gcnew System::Drawing::Font(fontName, size, style);
            if (font->Name == fontName) return font;
        }
        catch (...) { continue; }
    }
    return gcnew System::Drawing::Font("Segoe UI", size, style);
}

System::Drawing::Font^ GetEmojiFont(float size)
{
    return GetEmojiFont(size, FontStyle::Regular);
}

// ========== КИБЕР-КНОПКА ==========
public ref class CyberButton : public Button
{
private:
    Color baseColor;
    bool isHovered;
    Timer^ pulseTimer;
    float pulseIntensity;

public:
    CyberButton()
    {
        baseColor = Color::FromArgb(255, 215, 0);
        isHovered = false;
        pulseIntensity = 0.0f;

        this->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
        this->FlatAppearance->BorderSize = 0;
        this->Font = GetEmojiFont(10.0f, FontStyle::Bold);
        this->ForeColor = Color::White;
        this->Cursor = Cursors::Hand;
        this->BackColor = Color::FromArgb(20, 20, 30);
        this->SetStyle(ControlStyles::UserPaint | ControlStyles::AllPaintingInWmPaint |
            ControlStyles::OptimizedDoubleBuffer | ControlStyles::ResizeRedraw, true);

        pulseTimer = gcnew Timer();
        pulseTimer->Interval = 30;
        pulseTimer->Tick += gcnew EventHandler(this, &CyberButton::OnPulseTick);

        this->MouseEnter += gcnew EventHandler(this, &CyberButton::OnMouseEnter);
        this->MouseLeave += gcnew EventHandler(this, &CyberButton::OnMouseLeave);
    }

    property Color CyberColor {
        Color get() { return baseColor; }
        void set(Color value) { baseColor = value; this->Invalidate(); }
    }

    void OnMouseEnter(Object^ sender, EventArgs^ e) { isHovered = true; pulseTimer->Start(); }
    void OnMouseLeave(Object^ sender, EventArgs^ e) { isHovered = false; pulseTimer->Start(); }

    void OnPulseTick(Object^ sender, EventArgs^ e)
    {
        float target = isHovered ? 1.0f : 0.0f;
        pulseIntensity += (target - pulseIntensity) * 0.2f;
        if (Math::Abs(pulseIntensity - target) < 0.01f)
        {
            pulseIntensity = target;
            pulseTimer->Stop();
        }
        this->Invalidate();
    }

protected:
    virtual void OnPaint(PaintEventArgs^ e) override
    {
        e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
        e->Graphics->TextRenderingHint = Drawing::Text::TextRenderingHint::ClearTypeGridFit;

        System::Drawing::Rectangle rect = System::Drawing::Rectangle(2, 2, this->Width - 5, this->Height - 5);

        Color bgColor1 = Color::FromArgb(30, 30, 40);
        Color bgColor2 = Color::FromArgb(50, 50, 60);
        LinearGradientBrush^ bgBrush = gcnew LinearGradientBrush(rect, bgColor1, bgColor2, 90.0f);
        e->Graphics->FillRectangle(bgBrush, rect);

        int alpha = (int)(80 + 120 * pulseIntensity);
        Color cyberColor = Color::FromArgb(alpha, baseColor.R, baseColor.G, baseColor.B);
        Pen^ cyberPen = gcnew Pen(cyberColor, 2.0f + pulseIntensity * 2.0f);
        e->Graphics->DrawRectangle(cyberPen, rect);

        StringFormat^ sf = gcnew StringFormat();
        sf->Alignment = StringAlignment::Center;
        sf->LineAlignment = StringAlignment::Center;

        Color shadowColor = Color::FromArgb(100, 0, 0, 0);
        SolidBrush^ shadowBrush = gcnew SolidBrush(shadowColor);
        System::Drawing::Rectangle shadowRect = rect;
        shadowRect.Offset(1, 1);
        e->Graphics->DrawString(this->Text, this->Font, shadowBrush, shadowRect, sf);

        Color textColor = isHovered ? Color::White : Color::FromArgb(220, 220, 220);
        SolidBrush^ textBrush = gcnew SolidBrush(textColor);
        e->Graphics->DrawString(this->Text, this->Font, textBrush, rect, sf);

        delete bgBrush;
        delete cyberPen;
        delete textBrush;
        delete shadowBrush;
        delete sf;
    }
};

// ========== ТЕМНЫЙ DATAGRIDVIEW ==========
public ref class DarkDataGridView : public DataGridView
{
public:
    DarkDataGridView()
    {
        this->BackgroundColor = Color::FromArgb(25, 25, 35);
        this->BorderStyle = System::Windows::Forms::BorderStyle::None;
        this->EnableHeadersVisualStyles = false;
        this->RowHeadersVisible = false;
        this->AllowUserToResizeRows = false;
        this->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        this->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
        this->GridColor = Color::FromArgb(60, 60, 80);

        this->ColumnHeadersDefaultCellStyle->Font = GetEmojiFont(9.0f, FontStyle::Bold);
        this->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(35, 35, 50);
        this->ColumnHeadersDefaultCellStyle->ForeColor = Color::FromArgb(255, 215, 0);
        this->ColumnHeadersHeight = 40;

        this->DefaultCellStyle->Font = GetEmojiFont(9.0f);
        this->DefaultCellStyle->ForeColor = Color::FromArgb(230, 230, 230);
        this->DefaultCellStyle->BackColor = Color::FromArgb(45, 45, 60);
        this->DefaultCellStyle->SelectionBackColor = Color::FromArgb(218, 165, 32);
        this->DefaultCellStyle->SelectionForeColor = Color::Black;

        this->AlternatingRowsDefaultCellStyle->BackColor = Color::FromArgb(35, 35, 50);
        this->AlternatingRowsDefaultCellStyle->Font = GetEmojiFont(9.0f);

        this->RowTemplate->Height = 35;
        this->RowTemplate->DefaultCellStyle->Font = GetEmojiFont(9.0f);
    }
};

// ========== КИБЕР TEXTBOX ==========
public ref class CyberTextBox : public TextBox
{
public:
    CyberTextBox()
    {
        this->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
        this->Font = GetEmojiFont(10.0f);
        this->BackColor = Color::FromArgb(35, 35, 50);
        this->ForeColor = Color::FromArgb(230, 230, 230);
    }
};

// ========== КИБЕР КОМБОБОКС ==========
public ref class CyberComboBox : public ComboBox
{
public:
    CyberComboBox()
    {
        this->DropDownStyle = ComboBoxStyle::DropDownList;
        this->Font = GetEmojiFont(10.0f);
        this->BackColor = Color::FromArgb(35, 35, 50);
        this->ForeColor = Color::FromArgb(230, 230, 230);
        this->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
    }
};

// ========== ФОРМА УПРАВЛЕНИЯ КОМАНДАМИ ТУРНИРА ==========
public ref class TournamentTeamsForm : public Form
{
private:
    DatabaseManager* db;
    TeamRepository* teamRepo;
    int tournamentId;
    int tournamentSportId;
    DarkDataGridView^ allTeamsGrid;
    DarkDataGridView^ tournamentTeamsGrid;
    CyberButton^ btnAddTeam;
    CyberButton^ btnRemoveTeam;
    CyberButton^ btnClose;

public:
    TournamentTeamsForm(DatabaseManager* dbManager, int tournamentId)
    {
        db = dbManager;
        teamRepo = new TeamRepository(*db);
        this->tournamentId = tournamentId;

        tournamentSportId = 0;
        std::string sql = "SELECT sport_id FROM tournaments WHERE id = $1";
        std::string idStr = std::to_string(tournamentId);
        const char* params[1] = { idStr.c_str() };
        PGresult* res = PQexecParams(db->getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);
        if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0)
        {
            tournamentSportId = std::stoi(PQgetvalue(res, 0, 0));
        }
        PQclear(res);

        std::string sportName = "Неизвестно";
        if (tournamentSportId > 0)
        {
            sql = "SELECT name FROM sports WHERE id = $1";
            const char* params2[1] = { std::to_string(tournamentSportId).c_str() };
            res = PQexecParams(db->getConnection(), sql.c_str(), 1, NULL, params2, NULL, NULL, 0);
            if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0)
            {
                sportName = PQgetvalue(res, 0, 0);
            }
            PQclear(res);
        }

        InitializeComponent(sportName);
        LoadAllTeams();
        LoadTournamentTeams();
    }

    ~TournamentTeamsForm()
    {
        delete teamRepo;
    }

    void InitializeComponent(const std::string& sportName)
    {
        String^ disciplineName = gcnew String(sportName.c_str());

        this->Text = L"\U0001f465 Управление командами турнира — " + disciplineName;
        this->Size = System::Drawing::Size(1000, 650);
        this->StartPosition = FormStartPosition::CenterParent;
        this->BackColor = Color::FromArgb(20, 20, 35);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;

        Label^ lblInfo = gcnew Label();
        lblInfo->Text = L"Дисциплина: " + disciplineName;
        lblInfo->Font = GetEmojiFont(12.0f, FontStyle::Bold);
        lblInfo->ForeColor = Color::FromArgb(255, 215, 0);
        lblInfo->Location = Point(20, 15);
        lblInfo->AutoSize = true;

        Label^ lblAllTeams = gcnew Label();
        lblAllTeams->Text = L"\U0001f4cb Доступные команды (" + disciplineName + L")";
        lblAllTeams->Font = GetEmojiFont(12.0f, FontStyle::Bold);
        lblAllTeams->ForeColor = Color::FromArgb(59, 130, 246);
        lblAllTeams->Location = Point(20, 50);
        lblAllTeams->AutoSize = true;

        allTeamsGrid = gcnew DarkDataGridView();
        allTeamsGrid->Location = Point(20, 80);
        allTeamsGrid->Size = System::Drawing::Size(450, 440);
        allTeamsGrid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        allTeamsGrid->MultiSelect = true;

        Label^ lblTournamentTeams = gcnew Label();
        lblTournamentTeams->Text = L"\u2705 Команды-участники турнира";
        lblTournamentTeams->Font = GetEmojiFont(12.0f, FontStyle::Bold);
        lblTournamentTeams->ForeColor = Color::FromArgb(16, 185, 129);
        lblTournamentTeams->Location = Point(530, 50);
        lblTournamentTeams->AutoSize = true;

        tournamentTeamsGrid = gcnew DarkDataGridView();
        tournamentTeamsGrid->Location = Point(530, 80);
        tournamentTeamsGrid->Size = System::Drawing::Size(430, 440);
        tournamentTeamsGrid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
        tournamentTeamsGrid->MultiSelect = true;

        btnAddTeam = CreateActionButton(L"\u2795 Добавить в турнир \u2192", Color::FromArgb(16, 185, 129));
        btnAddTeam->Location = Point(140, 535);
        btnAddTeam->Size = System::Drawing::Size(200, 35);
        btnAddTeam->Click += gcnew EventHandler(this, &TournamentTeamsForm::HandleAddTeam);

        btnRemoveTeam = CreateActionButton(L"\u2190 \u2715 Убрать из турнира", Color::FromArgb(239, 68, 68));
        btnRemoveTeam->Location = Point(630, 535);
        btnRemoveTeam->Size = System::Drawing::Size(200, 35);
        btnRemoveTeam->Click += gcnew EventHandler(this, &TournamentTeamsForm::HandleRemoveTeam);

        btnClose = CreateActionButton(L"Закрыть", Color::FromArgb(100, 100, 120));
        btnClose->Location = Point(420, 535);
        btnClose->Size = System::Drawing::Size(120, 35);
        btnClose->Click += gcnew EventHandler(this, &TournamentTeamsForm::HandleClose);

        this->Controls->Add(lblInfo);
        this->Controls->Add(lblAllTeams);
        this->Controls->Add(allTeamsGrid);
        this->Controls->Add(lblTournamentTeams);
        this->Controls->Add(tournamentTeamsGrid);
        this->Controls->Add(btnAddTeam);
        this->Controls->Add(btnRemoveTeam);
        this->Controls->Add(btnClose);
    }

    CyberButton^ CreateActionButton(String^ text, Color color)
    {
        CyberButton^ btn = gcnew CyberButton();
        btn->Text = text;
        btn->CyberColor = color;
        btn->Font = GetEmojiFont(10.0f, FontStyle::Bold);
        return btn;
    }

    void LoadAllTeams()
    {
        allTeamsGrid->Rows->Clear();
        if (allTeamsGrid->Columns->Count == 0)
        {
            allTeamsGrid->Columns->Add(L"ID", L"ID");
            allTeamsGrid->Columns->Add(L"Name", L"Название");
            allTeamsGrid->Columns->Add(L"Tag", L"Тег");
            allTeamsGrid->Columns->Add(L"Wins", L"W");
            allTeamsGrid->Columns->Add(L"Losses", L"L");
            allTeamsGrid->Columns->Add(L"Points", L"Очки");
        }

        std::string sql = "SELECT t.id, t.name, t.tag, t.wins, t.losses, t.points "
            "FROM teams t "
            "WHERE t.sport_id = $1 "
            "ORDER BY t.name";

        std::string sportIdStr = std::to_string(tournamentSportId);
        const char* params[1] = { sportIdStr.c_str() };

        PGresult* res = PQexecParams(db->getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);
        if (PQresultStatus(res) == PGRES_TUPLES_OK)
        {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++)
            {
                allTeamsGrid->Rows->Add(
                    gcnew String(PQgetvalue(res, i, 0)),
                    gcnew String(PQgetvalue(res, i, 1)),
                    gcnew String(PQgetvalue(res, i, 2)),
                    gcnew String(PQgetvalue(res, i, 3)),
                    gcnew String(PQgetvalue(res, i, 4)),
                    gcnew String(PQgetvalue(res, i, 5))
                );
            }
        }
        PQclear(res);
    }

    void LoadTournamentTeams()
    {
        tournamentTeamsGrid->Rows->Clear();
        if (tournamentTeamsGrid->Columns->Count == 0)
        {
            tournamentTeamsGrid->Columns->Add(L"ID", L"ID");
            tournamentTeamsGrid->Columns->Add(L"Name", L"Название");
            tournamentTeamsGrid->Columns->Add(L"Tag", L"Тег");
            tournamentTeamsGrid->Columns->Add(L"Wins", L"W");
            tournamentTeamsGrid->Columns->Add(L"Losses", L"L");
            tournamentTeamsGrid->Columns->Add(L"Points", L"Очки");
        }

        std::vector<Team> teams = teamRepo->getTeamsByTournament(tournamentId);
        for (int i = 0; i < (int)teams.size(); i++)
        {
            Team t = teams[i];
            tournamentTeamsGrid->Rows->Add(
                t.getId(),
                gcnew String(t.getName().c_str()),
                gcnew String(t.getTag().c_str()),
                t.getWins(),
                t.getLosses(),
                t.getPoints()
            );
        }
    }

    void HandleAddTeam(Object^ sender, EventArgs^ e)
    {
        if (allTeamsGrid->SelectedRows->Count == 0)
        {
            MessageBox::Show(L"Выберите команды для добавления!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        int added = 0;
        for (int i = 0; i < allTeamsGrid->SelectedRows->Count; i++)
        {
            int teamId = Int32::Parse(allTeamsGrid->SelectedRows[i]->Cells[0]->Value->ToString());
            if (teamRepo->addTeamToTournament(teamId, tournamentId))
            {
                added++;
            }
        }

        LoadAllTeams();
        LoadTournamentTeams();
        MessageBox::Show(String::Format(L"Добавлено команд: {0}", added), L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void HandleRemoveTeam(Object^ sender, EventArgs^ e)
    {
        if (tournamentTeamsGrid->SelectedRows->Count == 0)
        {
            MessageBox::Show(L"Выберите команды для удаления!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        int removed = 0;
        for (int i = 0; i < tournamentTeamsGrid->SelectedRows->Count; i++)
        {
            int teamId = Int32::Parse(tournamentTeamsGrid->SelectedRows[i]->Cells[0]->Value->ToString());
            if (teamRepo->removeTeamFromTournament(teamId, tournamentId))
            {
                removed++;
            }
        }

        LoadAllTeams();
        LoadTournamentTeams();
        MessageBox::Show(String::Format(L"Убрано команд: {0}", removed), L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void HandleClose(Object^ sender, EventArgs^ e)
    {
        this->Close();
    }
};

// ========== ГЛАВНАЯ ФОРМА ==========
public ref class MainForm : public Form
{
private:
    DatabaseManager* db;
    SportRepository* sportRepo;
    TournamentRepository* tournamentRepo;
    TeamRepository* teamRepo;
    PlayerRepository* playerRepo;
    MatchRepository* matchRepo;

    Panel^ sidebarPanel;
    Panel^ mainPanel;
    Panel^ headerPanel;
    Label^ pageTitle;
    Panel^ contentPanel;

    CyberButton^ btnDashboard;
    CyberButton^ btnSports;
    CyberButton^ btnTournaments;
    CyberButton^ btnTeams;
    CyberButton^ btnPlayers;
    CyberButton^ btnMatches;

    int currentPage;

    // Dashboard cards
    Panel^ cardTotalSports;
    Panel^ cardTotalTournaments;
    Panel^ cardTotalTeams;
    Panel^ cardTotalPlayers;
    Panel^ cardTotalMatches;
    Panel^ cardLiveMatches;
    Label^ lblSportsValue;
    Label^ lblTournamentsValue;
    Label^ lblTeamsValue;
    Label^ lblPlayersValue;
    Label^ lblMatchesValue;
    Label^ lblLiveValue;

    // Dashboard filters
    CyberComboBox^ cmbDashboardSportFilter;

    // Dashboard additional panels
    DarkDataGridView^ upcomingMatchesGrid;
    DarkDataGridView^ activeTournamentsGrid;

    // Sports page
    DarkDataGridView^ sportsGrid;
    CyberTextBox^ txtSportName;
    CyberTextBox^ txtSportIcon;
    CyberComboBox^ cmbSportColor;
    CyberTextBox^ txtSportDescription;
    CyberTextBox^ txtSportId;
    CyberButton^ btnAddSport;
    CyberButton^ btnUpdateSport;
    CyberButton^ btnDeleteSport;

    // Tournaments page
    DarkDataGridView^ tournamentsGrid;
    CyberComboBox^ cmbTournamentFilterSport;
    CyberTextBox^ txtTournamentName;
    DateTimePicker^ dtpTournamentStart;
    DateTimePicker^ dtpTournamentEnd;
    CyberTextBox^ txtTournamentPrize;
    CyberComboBox^ cmbTournamentStatus;
    CyberTextBox^ txtTournamentId;
    CyberButton^ btnAddTournament;
    CyberButton^ btnUpdateTournament;
    CyberButton^ btnDeleteTournament;
    CyberButton^ btnStartTournament;
    CyberButton^ btnViewTournamentDetails;
    CyberButton^ btnManageTournamentTeams;

    Panel^ tournamentDetailPanel;
    Label^ lblTournamentDetailTitle;
    DarkDataGridView^ tournamentTeamsGrid;
    DarkDataGridView^ tournamentMatchesGrid;
    CyberButton^ btnBackToTournaments;
    int selectedTournamentId;

    // Teams page
    DarkDataGridView^ teamsGrid;
    CyberComboBox^ cmbTeamFilterSport;
    CyberTextBox^ txtTeamName;
    CyberTextBox^ txtTeamTag;
    CyberTextBox^ txtTeamLogo;
    CyberTextBox^ txtTeamId;
    CyberButton^ btnAddTeam;
    CyberButton^ btnUpdateTeam;
    CyberButton^ btnDeleteTeam;
    CyberButton^ btnViewTeamDetails;

    Panel^ teamDetailPanel;
    Label^ lblTeamDetailTitle;
    DarkDataGridView^ teamPlayersGrid;
    DarkDataGridView^ teamMatchesHistoryGrid;
    DarkDataGridView^ teamUpcomingMatchesGrid;
    CyberButton^ btnBackToTeams;
    int selectedTeamId;

    // Players page
    DarkDataGridView^ playersGrid;
    CyberComboBox^ cmbPlayerTeam;
    CyberTextBox^ txtPlayerNickname;
    CyberTextBox^ txtPlayerFullName;
    CyberTextBox^ txtPlayerRole;
    CyberTextBox^ txtPlayerNationality;
    CyberTextBox^ txtPlayerRating;
    CyberTextBox^ txtPlayerId;
    CyberButton^ btnAddPlayer;
    CyberButton^ btnUpdatePlayer;
    CyberButton^ btnDeletePlayer;

    // Matches page
    DarkDataGridView^ matchesGrid;
    CyberComboBox^ cmbMatchTournament;
    CyberComboBox^ cmbMatchTeam1;
    CyberComboBox^ cmbMatchTeam2;
    DateTimePicker^ dtpMatchDate;
    CyberTextBox^ txtMatchScore1;
    CyberTextBox^ txtMatchScore2;
    CyberComboBox^ cmbMatchStage;
    CyberComboBox^ cmbMatchStatus;
    CyberTextBox^ txtMatchId;
    CyberButton^ btnAddMatch;
    CyberButton^ btnUpdateMatch;
    CyberButton^ btnDeleteMatch;
    CyberButton^ btnStartMatch;
    CyberButton^ btnFinishMatch;

    // Match filters
    CyberComboBox^ cmbMatchFilterTournament;
    CyberComboBox^ cmbMatchFilterStatus;

    // Helper methods
    CyberComboBox^ FindCmbTeamSportForm()
    {
        for each (Control ^ ctrl in contentPanel->Controls)
        {
            if (ctrl->Tag != nullptr && (int)ctrl->Tag == 3)
            {
                Panel^ pagePanel = (Panel^)ctrl;
                for each (Control ^ childCtrl in pagePanel->Controls)
                {
                    if (childCtrl->GetType() == Panel::typeid)
                    {
                        Panel^ panel = (Panel^)childCtrl;
                        for each (Control ^ formCtrl in panel->Controls)
                        {
                            if (formCtrl->GetType() == CyberComboBox::typeid && formCtrl->Name == L"cmbTeamSportForm")
                            {
                                return (CyberComboBox^)formCtrl;
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    CyberComboBox^ FindCmbTournamentSportForm()
    {
        for each (Control ^ ctrl in contentPanel->Controls)
        {
            if (ctrl->Tag != nullptr && (int)ctrl->Tag == 2)
            {
                Panel^ pagePanel = (Panel^)ctrl;
                for each (Control ^ childCtrl in pagePanel->Controls)
                {
                    if (childCtrl->GetType() == Panel::typeid)
                    {
                        Panel^ panel = (Panel^)childCtrl;
                        for each (Control ^ formCtrl in panel->Controls)
                        {
                            if (formCtrl->GetType() == CyberComboBox::typeid && formCtrl->Name == L"cmbTournamentSportForm")
                            {
                                return (CyberComboBox^)formCtrl;
                            }
                        }
                    }
                }
            }
        }
        return nullptr;
    }

public:
    MainForm(DatabaseManager* dbManager)
    {
        db = dbManager;
        sportRepo = new SportRepository(*db);
        tournamentRepo = new TournamentRepository(*db);
        teamRepo = new TeamRepository(*db);
        playerRepo = new PlayerRepository(*db);
        matchRepo = new MatchRepository(*db);
        currentPage = 0;
        selectedTournamentId = 0;
        selectedTeamId = 0;

        InitializeForm();
        InitializeSidebar();
        InitializeMainPanel();
        InitializeAllPages();

        SwitchPage(0);

        this->Load += gcnew EventHandler(this, &MainForm::MainForm_Load);
    }

    ~MainForm()
    {
        if (sportRepo) delete sportRepo;
        if (tournamentRepo) delete tournamentRepo;
        if (teamRepo) delete teamRepo;
        if (playerRepo) delete playerRepo;
        if (matchRepo) delete matchRepo;
    }

    void InitializeForm()
    {
        this->Text = L"TOURNAMENT MANAGER — Система управления киберспортивными турнирами";
        this->Width = 1600;
        this->Height = 950;
        this->StartPosition = FormStartPosition::CenterScreen;
        this->BackColor = Color::FromArgb(15, 15, 25);
        this->Font = GetEmojiFont(10.0f);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
        this->MaximizeBox = false;
        this->Padding = System::Windows::Forms::Padding(0);
    }

    void InitializeSidebar()
    {
        sidebarPanel = gcnew Panel();
        sidebarPanel->Size = System::Drawing::Size(300, this->ClientSize.Height);
        sidebarPanel->BackColor = Color::FromArgb(200, 18, 18, 30);
        sidebarPanel->Dock = DockStyle::Left;

        Label^ logoLabel = gcnew Label();
        logoLabel->Text = L"\U0001f3c6 TOURNAMENT\n   MANAGER";
        logoLabel->Font = GetEmojiFont(20.0f, FontStyle::Bold);
        logoLabel->ForeColor = Color::FromArgb(255, 215, 0);
        logoLabel->Location = Point(25, 30);
        logoLabel->AutoSize = true;

        Panel^ separator = gcnew Panel();
        separator->Size = System::Drawing::Size(260, 1);
        separator->Location = Point(20, 120);
        separator->BackColor = Color::FromArgb(100, 100, 120);

        btnDashboard = CreateNavButton(L"\U0001f4ca Статистика", 150);
        btnSports = CreateNavButton(L"\U0001f3ae Виды спорта", 210);
        btnTournaments = CreateNavButton(L"\U0001f3c6 Турниры", 270);
        btnTeams = CreateNavButton(L"\U0001f465 Команды", 330);
        btnPlayers = CreateNavButton(L"\U0001f3af Игроки", 390);
        btnMatches = CreateNavButton(L"\u2694\ufe0f Матчи", 450);

        btnDashboard->Click += gcnew EventHandler(this, &MainForm::OnNavClick);
        btnSports->Click += gcnew EventHandler(this, &MainForm::OnNavClick);
        btnTournaments->Click += gcnew EventHandler(this, &MainForm::OnNavClick);
        btnTeams->Click += gcnew EventHandler(this, &MainForm::OnNavClick);
        btnPlayers->Click += gcnew EventHandler(this, &MainForm::OnNavClick);
        btnMatches->Click += gcnew EventHandler(this, &MainForm::OnNavClick);

        sidebarPanel->Controls->Add(logoLabel);
        sidebarPanel->Controls->Add(separator);
        sidebarPanel->Controls->Add(btnDashboard);
        sidebarPanel->Controls->Add(btnSports);
        sidebarPanel->Controls->Add(btnTournaments);
        sidebarPanel->Controls->Add(btnTeams);
        sidebarPanel->Controls->Add(btnPlayers);
        sidebarPanel->Controls->Add(btnMatches);

        this->Controls->Add(sidebarPanel);
    }

    CyberButton^ CreateNavButton(String^ text, int y)
    {
        CyberButton^ btn = gcnew CyberButton();
        btn->Text = text;
        btn->Size = System::Drawing::Size(260, 45);
        btn->Location = Point(20, y);
        btn->CyberColor = Color::FromArgb(255, 215, 0);
        btn->Font = GetEmojiFont(12.0f, FontStyle::Regular);
        btn->TextAlign = ContentAlignment::MiddleLeft;
        btn->Padding = System::Windows::Forms::Padding(15, 0, 0, 0);
        return btn;
    }

    void InitializeMainPanel()
    {
        mainPanel = gcnew Panel();
        mainPanel->Size = System::Drawing::Size(this->ClientSize.Width - 300, this->ClientSize.Height);
        mainPanel->Location = Point(300, 0);
        mainPanel->BackColor = Color::Transparent;
        mainPanel->Padding = System::Windows::Forms::Padding(25, 20, 25, 20);

        headerPanel = gcnew Panel();
        headerPanel->Size = System::Drawing::Size(mainPanel->Width - 50, 70);
        headerPanel->Location = Point(0, 0);
        headerPanel->BackColor = Color::Transparent;

        pageTitle = gcnew Label();
        pageTitle->Text = L"Статистика";
        pageTitle->Font = GetEmojiFont(28.0f, FontStyle::Bold);
        pageTitle->ForeColor = Color::FromArgb(255, 215, 0);
        pageTitle->Location = Point(0, 10);
        pageTitle->AutoSize = true;

        headerPanel->Controls->Add(pageTitle);

        contentPanel = gcnew Panel();
        contentPanel->Size = System::Drawing::Size(mainPanel->Width - 50, mainPanel->Height - 110);
        contentPanel->Location = Point(0, 80);
        contentPanel->BackColor = Color::FromArgb(200, 20, 20, 35);
        contentPanel->AutoScroll = true;

        mainPanel->Controls->Add(headerPanel);
        mainPanel->Controls->Add(contentPanel);
        this->Controls->Add(mainPanel);
    }

    void InitializeAllPages()
    {
        InitializeDashboardPage();
        InitializeSportsPage();
        InitializeTournamentsPage();
        InitializeTournamentDetailPage();
        InitializeTeamsPage();
        InitializeTeamDetailPage();
        InitializePlayersPage();
        InitializeMatchesPage();
    }

    // ========== ДАШБОРД ==========
    void InitializeDashboardPage()
    {
        Panel^ dashPanel = gcnew Panel();
        dashPanel->Size = System::Drawing::Size(contentPanel->Width, 1200);
        dashPanel->BackColor = Color::Transparent;
        dashPanel->Tag = (Object^)0;
        dashPanel->Visible = false;
        dashPanel->AutoScroll = true;

        // Карточки статистики
        int cardWidth = (contentPanel->Width - 80) / 6;
        if (cardWidth < 150) cardWidth = 150;

        cardTotalSports = CreateStatCard(cardWidth, L"\U0001f3ae", L"ВИДОВ СПОРТА", L"0", 0, Color::FromArgb(59, 130, 246));
        lblSportsValue = (Label^)cardTotalSports->Controls["valueLabel"];

        cardTotalTournaments = CreateStatCard(cardWidth, L"\U0001f3c6", L"ТУРНИРОВ", L"0", cardWidth + 15, Color::FromArgb(16, 185, 129));
        lblTournamentsValue = (Label^)cardTotalTournaments->Controls["valueLabel"];

        cardTotalTeams = CreateStatCard(cardWidth, L"\U0001f465", L"КОМАНД", L"0", (cardWidth + 15) * 2, Color::FromArgb(245, 158, 11));
        lblTeamsValue = (Label^)cardTotalTeams->Controls["valueLabel"];

        cardTotalPlayers = CreateStatCard(cardWidth, L"\U0001f3af", L"ИГРОКОВ", L"0", (cardWidth + 15) * 3, Color::FromArgb(139, 92, 246));
        lblPlayersValue = (Label^)cardTotalPlayers->Controls["valueLabel"];

        cardTotalMatches = CreateStatCard(cardWidth, L"\u2694\ufe0f", L"МАТЧЕЙ", L"0", (cardWidth + 15) * 4, Color::FromArgb(236, 72, 153));
        lblMatchesValue = (Label^)cardTotalMatches->Controls["valueLabel"];

        cardLiveMatches = CreateStatCard(cardWidth, L"\U0001f525", L"LIVE", L"0", (cardWidth + 15) * 5, Color::FromArgb(239, 68, 68));
        lblLiveValue = (Label^)cardLiveMatches->Controls["valueLabel"];

        // Фильтр по дисциплинам
        Label^ lblDashFilter = gcnew Label();
        lblDashFilter->Text = L"\U0001f3ae Фильтр по дисциплине:";
        lblDashFilter->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        lblDashFilter->ForeColor = Color::FromArgb(255, 215, 0);
        lblDashFilter->Location = Point(0, 120);
        lblDashFilter->AutoSize = true;

        cmbDashboardSportFilter = gcnew CyberComboBox();
        cmbDashboardSportFilter->Location = Point(250, 118);
        cmbDashboardSportFilter->Size = System::Drawing::Size(300, 30);
        cmbDashboardSportFilter->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnDashboardSportFilterChanged);

        // Ближайшие матчи
        Label^ lblUpcomingMatches = gcnew Label();
        lblUpcomingMatches->Text = L"\U0001f4c5 Ближайшие матчи";
        lblUpcomingMatches->Font = GetEmojiFont(16.0f, FontStyle::Bold);
        lblUpcomingMatches->ForeColor = Color::FromArgb(59, 130, 246);
        lblUpcomingMatches->Location = Point(0, 170);
        lblUpcomingMatches->AutoSize = true;

        upcomingMatchesGrid = gcnew DarkDataGridView();
        upcomingMatchesGrid->Location = Point(0, 210);
        upcomingMatchesGrid->Size = System::Drawing::Size(contentPanel->Width - 20, 350);

        // Активные турниры
        Label^ lblActiveTournaments = gcnew Label();
        lblActiveTournaments->Text = L"\U0001f525 Активные турниры";
        lblActiveTournaments->Font = GetEmojiFont(16.0f, FontStyle::Bold);
        lblActiveTournaments->ForeColor = Color::FromArgb(239, 68, 68);
        lblActiveTournaments->Location = Point(0, 580);
        lblActiveTournaments->AutoSize = true;

        activeTournamentsGrid = gcnew DarkDataGridView();
        activeTournamentsGrid->Location = Point(0, 620);
        activeTournamentsGrid->Size = System::Drawing::Size(contentPanel->Width - 20, 300);

        dashPanel->Controls->Add(cardTotalSports);
        dashPanel->Controls->Add(cardTotalTournaments);
        dashPanel->Controls->Add(cardTotalTeams);
        dashPanel->Controls->Add(cardTotalPlayers);
        dashPanel->Controls->Add(cardTotalMatches);
        dashPanel->Controls->Add(cardLiveMatches);
        dashPanel->Controls->Add(lblDashFilter);
        dashPanel->Controls->Add(cmbDashboardSportFilter);
        dashPanel->Controls->Add(lblUpcomingMatches);
        dashPanel->Controls->Add(upcomingMatchesGrid);
        dashPanel->Controls->Add(lblActiveTournaments);
        dashPanel->Controls->Add(activeTournamentsGrid);

        contentPanel->Controls->Add(dashPanel);
    }

    Panel^ CreateStatCard(int width, String^ icon, String^ title, String^ value, int x, Color accentColor)
    {
        Panel^ card = gcnew Panel();
        card->Size = System::Drawing::Size(width, 100);
        card->Location = Point(x, 0);
        card->BackColor = Color::FromArgb(40, 40, 55);

        Label^ iconLabel = gcnew Label();
        iconLabel->Text = icon;
        iconLabel->Font = GetEmojiFont(28.0f);
        iconLabel->ForeColor = accentColor;
        iconLabel->Location = Point(15, 30);
        iconLabel->AutoSize = true;

        Label^ valueLabel = gcnew Label();
        valueLabel->Text = value;
        valueLabel->Font = GetEmojiFont(24.0f, FontStyle::Bold);
        valueLabel->ForeColor = Color::White;
        valueLabel->TextAlign = ContentAlignment::TopRight;
        valueLabel->Location = Point(width - 120, 15);
        valueLabel->Size = System::Drawing::Size(105, 35);
        valueLabel->Name = L"valueLabel";

        Label^ titleLabel = gcnew Label();
        titleLabel->Text = title;
        titleLabel->Font = GetEmojiFont(9.0f);
        titleLabel->ForeColor = Color::FromArgb(180, 180, 200);
        titleLabel->TextAlign = ContentAlignment::TopRight;
        titleLabel->Location = Point(width - 150, 55);
        titleLabel->Size = System::Drawing::Size(135, 25);

        card->Controls->Add(iconLabel);
        card->Controls->Add(valueLabel);
        card->Controls->Add(titleLabel);

        return card;
    }

    Label^ CreateFormLabel(String^ text, int y)
    {
        Label^ label = gcnew Label();
        label->Text = text;
        label->Location = Point(20, y);
        label->Font = GetEmojiFont(9.0f, FontStyle::Bold);
        label->ForeColor = Color::FromArgb(200, 200, 220);
        label->AutoSize = true;
        return label;
    }

    CyberTextBox^ CreateFormTextBox(bool readOnly)
    {
        CyberTextBox^ tb = gcnew CyberTextBox();
        tb->ReadOnly = readOnly;
        tb->Font = GetEmojiFont(10.0f);
        return tb;
    }

    CyberButton^ CreateActionButton(String^ text, Color color)
    {
        CyberButton^ btn = gcnew CyberButton();
        btn->Text = text;
        btn->CyberColor = color;
        btn->Font = GetEmojiFont(10.0f, FontStyle::Bold);
        return btn;
    }

    String^ ConvertUtf8ToString(const std::string& utf8Str)
    {
        if (utf8Str.empty()) return String::Empty;
        try
        {
            array<Byte>^ bytes = gcnew array<Byte>((int)utf8Str.length());
            for (int i = 0; i < (int)utf8Str.length(); i++)
            {
                bytes[i] = (Byte)utf8Str[i];
            }
            return System::Text::Encoding::UTF8->GetString(bytes);
        }
        catch (...)
        {
            return gcnew String(utf8Str.c_str());
        }
    }
    void LoadAllFilters()
    {
        LoadTournamentSportFilter();
        LoadTeamSportFilter();
        LoadTournamentSportFormCombo();
        LoadDashboardSportFilter();
        LoadMatchFilters();
        LoadComboBoxes();
    }

    void LoadDashboardSportFilter()
    {
        cmbDashboardSportFilter->Items->Clear();
        cmbDashboardSportFilter->Items->Add(L"Все дисциплины");
        std::vector<Sport> sports = sportRepo->getAllSports();
        for (int i = 0; i < (int)sports.size(); i++)
        {
            Sport s = sports[i];
            cmbDashboardSportFilter->Items->Add(gcnew String((std::to_string(s.getId()) + " - " + s.getName()).c_str()));
        }
        cmbDashboardSportFilter->SelectedIndex = 0;
    }

    void LoadTournamentSportFormCombo()
    {
        CyberComboBox^ cmbTournamentSportForm = FindCmbTournamentSportForm();
        if (cmbTournamentSportForm != nullptr)
        {
            cmbTournamentSportForm->Items->Clear();
            std::vector<Sport> sports = sportRepo->getAllSports();
            for (int i = 0; i < (int)sports.size(); i++)
            {
                Sport s = sports[i];
                cmbTournamentSportForm->Items->Add(gcnew String((std::to_string(s.getId()) + " - " + s.getName()).c_str()));
            }
            if (cmbTournamentSportForm->Items->Count > 0) cmbTournamentSportForm->SelectedIndex = 0;
        }
    }

    // ========== НАВИГАЦИЯ ==========
    void OnNavClick(Object^ sender, EventArgs^ e)
    {
        CyberButton^ btn = (CyberButton^)sender;
        int page = 0;

        if (btn == btnDashboard) page = 0;
        else if (btn == btnSports) page = 1;
        else if (btn == btnTournaments) { page = 2; LoadTournamentSportFilter(); LoadTournamentSportFormCombo(); }
        else if (btn == btnTeams) { page = 3; LoadTeamSportFilter(); }
        else if (btn == btnPlayers) page = 4;
        else if (btn == btnMatches) { page = 5; LoadMatchFilters(); }

        SwitchPage(page);
    }

    void SwitchPage(int page)
    {
        currentPage = page;

        array<String^>^ titles = { L"\U0001f4ca Статистика", L"\U0001f3ae Виды спорта", L"\U0001f3c6 Турниры", L"\U0001f465 Команды", L"\U0001f3af Игроки", L"\u2694\ufe0f Матчи" };
        pageTitle->Text = titles[page];

        for each (Control ^ ctrl in contentPanel->Controls)
        {
            if (ctrl->Tag != nullptr)
            {
                int tag = (int)ctrl->Tag;
                ctrl->Visible = (tag == page);
            }
        }

        RefreshCurrentPage();
    }

    void RefreshCurrentPage()
    {
        switch (currentPage)
        {
        case 0: UpdateDashboardStats(); break;
        case 1: LoadSports(); break;
        case 2: LoadFilteredTournaments(); break;
        case 3: LoadFilteredTeams(); break;
        case 4: LoadPlayers(); break;
        case 5: LoadFilteredMatches(); break;
        }
    }

    void MainForm_Load(Object^ sender, EventArgs^ e)
    {
        LoadAllData();
        LoadComboBoxes();
        LoadAllFilters();
        UpdateDashboardStats();
    }

    void LoadAllData()
    {
        LoadSports();
        LoadTournaments();
        LoadTeams();
        LoadPlayers();
        LoadFilteredMatches();
    }

    // ========== ДАШБОРД (фильтры и загрузка данных) ==========
    void OnDashboardSportFilterChanged(Object^ sender, EventArgs^ e)
    {
        UpdateDashboardStats();
    }

    void UpdateDashboardStats()
    {
        std::vector<Sport> sports = sportRepo->getAllSports();
        std::vector<Tournament> tournaments = tournamentRepo->getAllTournaments();
        std::vector<Team> teams = teamRepo->getAllTeams();
        std::vector<Player> players = playerRepo->getAllPlayers();
        std::vector<Match> matches = matchRepo->getAllMatches();

        int selectedSportId = 0;
        if (cmbDashboardSportFilter->SelectedIndex > 0)
        {
            String^ selected = cmbDashboardSportFilter->SelectedItem->ToString();
            selectedSportId = Int32::Parse(selected->Substring(0, selected->IndexOf(L" ")));
        }

        if (selectedSportId > 0)
        {
            // Фильтруем по дисциплине
            std::vector<Tournament> filteredTournaments;
            std::vector<Team> filteredTeams;
            std::vector<Match> filteredMatches;

            for (auto& t : tournaments) {
                if (t.getSportId() == selectedSportId) filteredTournaments.push_back(t);
            }
            for (auto& t : teams) {
                if (t.getSportId() == selectedSportId) filteredTeams.push_back(t);
            }
            for (auto& m : matches) {
                for (auto& t : filteredTournaments) {
                    if (m.getTournamentId() == t.getId()) {
                        filteredMatches.push_back(m);
                        break;
                    }
                }
            }

            lblSportsValue->Text = "1";
            lblTournamentsValue->Text = filteredTournaments.size().ToString();
            lblTeamsValue->Text = filteredTeams.size().ToString();
            lblMatchesValue->Text = filteredMatches.size().ToString();

            int liveCount = 0;
            for (auto& m : filteredMatches) {
                if (m.getStatus() == "live") liveCount++;
            }
            lblLiveValue->Text = liveCount.ToString();
        }
        else
        {
            lblSportsValue->Text = sports.size().ToString();
            lblTournamentsValue->Text = tournaments.size().ToString();
            lblTeamsValue->Text = teams.size().ToString();
            lblPlayersValue->Text = players.size().ToString();
            lblMatchesValue->Text = matches.size().ToString();

            int liveCount = 0;
            for (int i = 0; i < (int)matches.size(); i++)
            {
                if (matches[i].getStatus() == "live") liveCount++;
            }
            lblLiveValue->Text = liveCount.ToString();
        }

        // Загружаем ближайшие матчи
        LoadUpcomingMatches(selectedSportId);

        // Загружаем активные турниры
        LoadActiveTournaments(selectedSportId);
    }

    void LoadUpcomingMatches(int sportId)
    {
        upcomingMatchesGrid->Rows->Clear();
        if (upcomingMatchesGrid->Columns->Count == 0)
        {
            upcomingMatchesGrid->Columns->Add(L"ID", L"ID");
            upcomingMatchesGrid->Columns->Add(L"Tournament", L"Турнир");
            upcomingMatchesGrid->Columns->Add(L"Sport", L"Дисциплина");
            upcomingMatchesGrid->Columns->Add(L"Team 1", L"Команда 1");
            upcomingMatchesGrid->Columns->Add(L"Team 2", L"Команда 2");
            upcomingMatchesGrid->Columns->Add(L"Date", L"Дата");
            upcomingMatchesGrid->Columns->Add(L"Stage", L"Стадия");
            upcomingMatchesGrid->Columns->Add(L"Status", L"Статус");
        }

        std::vector<Match> upcomingMatches;
        if (sportId > 0)
        {
            upcomingMatches = tournamentRepo->getUpcomingMatchesBySport(sportId, 10);
        }
        else
        {
            // Получаем все матчи и фильтруем scheduled с датой > сейчас
            std::vector<Match> allMatches = matchRepo->getAllMatches();
            for (auto& m : allMatches)
            {
                if (m.getStatus() == "scheduled")
                {
                    upcomingMatches.push_back(m);
                    if (upcomingMatches.size() >= 10) break;
                }
            }
        }

        for (int i = 0; i < (int)upcomingMatches.size(); i++)
        {
            Match m = upcomingMatches[i];
            Tournament t = tournamentRepo->getTournamentById(m.getTournamentId());
            std::string sportName = sportRepo->getSportName(t.getSportId());

            upcomingMatchesGrid->Rows->Add(
                m.getId(),
                gcnew String(t.getName().c_str()),
                gcnew String(sportName.c_str()),
                gcnew String(teamRepo->getTeamDisplayName(m.getTeam1Id()).c_str()),
                gcnew String(teamRepo->getTeamDisplayName(m.getTeam2Id()).c_str()),
                gcnew String(m.getMatchDate().c_str()),
                gcnew String(m.getStage().c_str()),
                gcnew String(m.getStatus().c_str())
            );
        }
    }

    void LoadActiveTournaments(int sportId)
    {
        activeTournamentsGrid->Rows->Clear();
        if (activeTournamentsGrid->Columns->Count == 0)
        {
            activeTournamentsGrid->Columns->Add(L"ID", L"ID");
            activeTournamentsGrid->Columns->Add(L"Name", L"Название");
            activeTournamentsGrid->Columns->Add(L"Sport", L"Дисциплина");
            activeTournamentsGrid->Columns->Add(L"Start", L"Начало");
            activeTournamentsGrid->Columns->Add(L"End", L"Конец");
            activeTournamentsGrid->Columns->Add(L"Prize", L"Призовые");
            activeTournamentsGrid->Columns->Add(L"Status", L"Статус");
        }

        std::vector<Tournament> activeTournaments;
        if (sportId > 0)
        {
            std::vector<Tournament> allActive = tournamentRepo->getTournamentsByStatus("active");
            for (auto& t : allActive)
            {
                if (t.getSportId() == sportId)
                {
                    activeTournaments.push_back(t);
                }
            }
        }
        else
        {
            activeTournaments = tournamentRepo->getTournamentsByStatus("active");
        }

        for (int i = 0; i < (int)activeTournaments.size(); i++)
        {
            Tournament t = activeTournaments[i];
            std::string sportName = sportRepo->getSportName(t.getSportId());

            activeTournamentsGrid->Rows->Add(
                t.getId(),
                gcnew String(t.getName().c_str()),
                gcnew String(sportName.c_str()),
                gcnew String(t.getStartDate().c_str()),
                gcnew String(t.getEndDate().c_str()),
                String::Format(L"${0:F2}", t.getPrizePool()),
                gcnew String(t.getStatus().c_str())
            );
        }
    }

    void LoadSports()
    {
        sportsGrid->Rows->Clear();
        if (sportsGrid->Columns->Count == 0)
        {
            sportsGrid->Columns->Add(L"ID", L"ID");
            sportsGrid->Columns->Add(L"Name", L"Название");
            sportsGrid->Columns->Add(L"Icon", L"Иконка");
            sportsGrid->Columns->Add(L"Color", L"Цвет");
        }

        std::vector<Sport> sports = sportRepo->getAllSports();
        for (int i = 0; i < (int)sports.size(); i++)
        {
            Sport s = sports[i];
            sportsGrid->Rows->Add(s.getId(), gcnew String(s.getName().c_str()), ConvertUtf8ToString(s.getIcon()), gcnew String(s.getColor().c_str()));
        }
    }

    void LoadTournaments()
    {
        tournamentsGrid->Rows->Clear();
        if (tournamentsGrid->Columns->Count == 0)
        {
            tournamentsGrid->Columns->Add(L"ID", L"ID");
            tournamentsGrid->Columns->Add(L"Sport", L"Дисциплина");
            tournamentsGrid->Columns->Add(L"Name", L"Название");
            tournamentsGrid->Columns->Add(L"Start", L"Начало");
            tournamentsGrid->Columns->Add(L"End", L"Конец");
            tournamentsGrid->Columns->Add(L"Prize", L"Призовые");
            tournamentsGrid->Columns->Add(L"Status", L"Статус");
        }

        std::vector<Tournament> tournaments = tournamentRepo->getAllTournaments();
        for (int i = 0; i < (int)tournaments.size(); i++)
        {
            Tournament t = tournaments[i];
            tournamentsGrid->Rows->Add(t.getId(), gcnew String(sportRepo->getSportName(t.getSportId()).c_str()), gcnew String(t.getName().c_str()), gcnew String(t.getStartDate().c_str()), gcnew String(t.getEndDate().c_str()), String::Format(L"${0:F2}", t.getPrizePool()), gcnew String(t.getStatus().c_str()));
        }
    }

    void LoadFilteredTournaments()
    {
        tournamentsGrid->Rows->Clear();
        if (tournamentsGrid->Columns->Count == 0)
        {
            tournamentsGrid->Columns->Add(L"ID", L"ID");
            tournamentsGrid->Columns->Add(L"Sport", L"Дисциплина");
            tournamentsGrid->Columns->Add(L"Name", L"Название");
            tournamentsGrid->Columns->Add(L"Start", L"Начало");
            tournamentsGrid->Columns->Add(L"End", L"Конец");
            tournamentsGrid->Columns->Add(L"Prize", L"Призовые");
            tournamentsGrid->Columns->Add(L"Status", L"Статус");
        }

        std::vector<Tournament> tournaments;
        if (cmbTournamentFilterSport->SelectedIndex > 0)
        {
            String^ selected = cmbTournamentFilterSport->SelectedItem->ToString();
            int sportId = Int32::Parse(selected->Substring(0, selected->IndexOf(L" ")));
            tournaments = tournamentRepo->getTournamentsBySport(sportId);
        }
        else
        {
            tournaments = tournamentRepo->getAllTournaments();
        }

        for (int i = 0; i < (int)tournaments.size(); i++)
        {
            Tournament t = tournaments[i];
            tournamentsGrid->Rows->Add(t.getId(), gcnew String(sportRepo->getSportName(t.getSportId()).c_str()), gcnew String(t.getName().c_str()), gcnew String(t.getStartDate().c_str()), gcnew String(t.getEndDate().c_str()), String::Format(L"${0:F2}", t.getPrizePool()), gcnew String(t.getStatus().c_str()));
        }
    }

    void LoadTournamentSportFilter()
    {
        cmbTournamentFilterSport->Items->Clear();
        cmbTournamentFilterSport->Items->Add(L"Все дисциплины");
        std::vector<Sport> sports = sportRepo->getAllSports();
        for (int i = 0; i < (int)sports.size(); i++)
        {
            Sport s = sports[i];
            cmbTournamentFilterSport->Items->Add(gcnew String((std::to_string(s.getId()) + " - " + s.getName()).c_str()));
        }
        cmbTournamentFilterSport->SelectedIndex = 0;
    }

    void OnTournamentSportFilterChanged(Object^ sender, EventArgs^ e)
    {
        LoadFilteredTournaments();
    }

    void LoadTeams()
    {
        teamsGrid->Rows->Clear();
        if (teamsGrid->Columns->Count == 0)
        {
            teamsGrid->Columns->Add(L"ID", L"ID");
            teamsGrid->Columns->Add(L"Sport", L"Дисциплина");
            teamsGrid->Columns->Add(L"Name", L"Название");
            teamsGrid->Columns->Add(L"Tag", L"Тег");
            teamsGrid->Columns->Add(L"Logo", L"Лого");
            teamsGrid->Columns->Add(L"W", L"W");
            teamsGrid->Columns->Add(L"L", L"L");
            teamsGrid->Columns->Add(L"Pts", L"Очки");
        }

        std::vector<Team> teams = teamRepo->getAllTeams();
        for (int i = 0; i < (int)teams.size(); i++)
        {
            Team t = teams[i];
            String^ sportName = gcnew String(sportRepo->getSportName(t.getSportId()).c_str());
            if (sportName == L"Unknown")
            {
                Sport sport = sportRepo->getSportById(t.getSportId());
                if (sport.getId() > 0) sportName = gcnew String(sport.getName().c_str());
            }
            teamsGrid->Rows->Add(t.getId(), sportName, gcnew String(t.getName().c_str()), gcnew String(t.getTag().c_str()), ConvertUtf8ToString(t.getLogo()), t.getWins(), t.getLosses(), t.getPoints());
        }
    }

    void LoadFilteredTeams()
    {
        teamsGrid->Rows->Clear();
        if (teamsGrid->Columns->Count == 0)
        {
            teamsGrid->Columns->Add(L"ID", L"ID");
            teamsGrid->Columns->Add(L"Sport", L"Дисциплина");
            teamsGrid->Columns->Add(L"Name", L"Название");
            teamsGrid->Columns->Add(L"Tag", L"Тег");
            teamsGrid->Columns->Add(L"Logo", L"Лого");
            teamsGrid->Columns->Add(L"W", L"W");
            teamsGrid->Columns->Add(L"L", L"L");
            teamsGrid->Columns->Add(L"Pts", L"Очки");
        }

        std::vector<Team> teams;
        if (cmbTeamFilterSport->SelectedIndex > 0)
        {
            String^ selected = cmbTeamFilterSport->SelectedItem->ToString();
            int sportId = Int32::Parse(selected->Substring(0, selected->IndexOf(L" ")));
            teams = teamRepo->getTeamsBySport(sportId);
        }
        else
        {
            teams = teamRepo->getAllTeams();
        }

        for (int i = 0; i < (int)teams.size(); i++)
        {
            Team t = teams[i];
            String^ sportName = gcnew String(sportRepo->getSportName(t.getSportId()).c_str());
            if (sportName == L"Unknown")
            {
                Sport sport = sportRepo->getSportById(t.getSportId());
                if (sport.getId() > 0) sportName = gcnew String(sport.getName().c_str());
            }
            teamsGrid->Rows->Add(t.getId(), sportName, gcnew String(t.getName().c_str()), gcnew String(t.getTag().c_str()), ConvertUtf8ToString(t.getLogo()), t.getWins(), t.getLosses(), t.getPoints());
        }
    }

    void LoadTeamSportFilter()
    {
        cmbTeamFilterSport->Items->Clear();
        cmbTeamFilterSport->Items->Add(L"Все дисциплины");
        std::vector<Sport> sports = sportRepo->getAllSports();
        for (int i = 0; i < (int)sports.size(); i++)
        {
            Sport s = sports[i];
            cmbTeamFilterSport->Items->Add(gcnew String((std::to_string(s.getId()) + " - " + s.getName()).c_str()));
        }
        cmbTeamFilterSport->SelectedIndex = 0;

        CyberComboBox^ cmbTeamSportForm = FindCmbTeamSportForm();
        if (cmbTeamSportForm != nullptr)
        {
            cmbTeamSportForm->Items->Clear();
            for (int i = 0; i < (int)sports.size(); i++)
            {
                Sport s = sports[i];
                cmbTeamSportForm->Items->Add(gcnew String((std::to_string(s.getId()) + " - " + s.getName()).c_str()));
            }
            if (cmbTeamSportForm->Items->Count > 0) cmbTeamSportForm->SelectedIndex = 0;
        }
    }

    void OnTeamSportFilterChanged(Object^ sender, EventArgs^ e)
    {
        LoadFilteredTeams();
    }

    void LoadPlayers()
    {
        playersGrid->Rows->Clear();
        if (playersGrid->Columns->Count == 0)
        {
            playersGrid->Columns->Add(L"ID", L"ID");
            playersGrid->Columns->Add(L"Team", L"Команда");
            playersGrid->Columns->Add(L"Nickname", L"Никнейм");
            playersGrid->Columns->Add(L"Full Name", L"Полное имя");
            playersGrid->Columns->Add(L"Role", L"Роль");
            playersGrid->Columns->Add(L"Nationality", L"Национальность");
            playersGrid->Columns->Add(L"Rating", L"Рейтинг");
        }

        std::vector<Player> players = playerRepo->getAllPlayers();
        for (int i = 0; i < (int)players.size(); i++)
        {
            Player p = players[i];
            playersGrid->Rows->Add(p.getId(), p.getTeamId() > 0 ? gcnew String(teamRepo->getTeamDisplayName(p.getTeamId()).c_str()) : L"Free Agent", gcnew String(p.getNickname().c_str()), gcnew String(p.getFullName().c_str()), gcnew String(p.getRole().c_str()), gcnew String(p.getNationality().c_str()), p.getRating());
        }
    }

    // ========== МАТЧИ (фильтры) ==========
    void LoadMatchFilters()
    {
        cmbMatchFilterTournament->Items->Clear();
        cmbMatchFilterTournament->Items->Add(L"Все турниры");
        std::vector<Tournament> tournaments = tournamentRepo->getAllTournaments();
        for (int i = 0; i < (int)tournaments.size(); i++)
        {
            Tournament t = tournaments[i];
            cmbMatchFilterTournament->Items->Add(gcnew String((std::to_string(t.getId()) + " - " + t.getName()).c_str()));
        }
        cmbMatchFilterTournament->SelectedIndex = 0;

        cmbMatchFilterStatus->Items->Clear();
        cmbMatchFilterStatus->Items->Add(L"Все статусы");
        cmbMatchFilterStatus->Items->Add(L"scheduled");
        cmbMatchFilterStatus->Items->Add(L"live");
        cmbMatchFilterStatus->Items->Add(L"completed");
        cmbMatchFilterStatus->SelectedIndex = 0;
    }

    void OnMatchFilterChanged(Object^ sender, EventArgs^ e)
    {
        LoadFilteredMatches();
    }

    void OnResetMatchFilters(Object^ sender, EventArgs^ e)
    {
        cmbMatchFilterTournament->SelectedIndex = 0;
        cmbMatchFilterStatus->SelectedIndex = 0;
        LoadFilteredMatches();
    }

    void LoadFilteredMatches()
    {
        matchesGrid->Rows->Clear();
        if (matchesGrid->Columns->Count == 0)
        {
            matchesGrid->Columns->Add(L"ID", L"ID");
            matchesGrid->Columns->Add(L"Tournament", L"Турнир");
            matchesGrid->Columns->Add(L"Team 1", L"Команда 1");
            matchesGrid->Columns->Add(L"Team 2", L"Команда 2");
            matchesGrid->Columns->Add(L"Score", L"Счет");
            matchesGrid->Columns->Add(L"Date", L"Дата");
            matchesGrid->Columns->Add(L"Stage", L"Стадия");
            matchesGrid->Columns->Add(L"Status", L"Статус");
        }

        std::vector<Match> matches;

        if (cmbMatchFilterTournament->SelectedIndex > 0 && cmbMatchFilterStatus->SelectedIndex > 0)
        {
            String^ tournamentStr = cmbMatchFilterTournament->SelectedItem->ToString();
            int tournamentId = Int32::Parse(tournamentStr->Substring(0, tournamentStr->IndexOf(L" ")));
            String^ statusFilter = cmbMatchFilterStatus->SelectedItem->ToString();

            std::vector<Match> allMatches = matchRepo->getAllMatches();
            for (int i = 0; i < (int)allMatches.size(); i++)
            {
                Match m = allMatches[i];
                if (m.getTournamentId() == tournamentId && m.getStatus() == msclr::interop::marshal_as<std::string>(statusFilter))
                {
                    matches.push_back(m);
                }
            }
        }
        else if (cmbMatchFilterTournament->SelectedIndex > 0)
        {
            String^ tournamentStr = cmbMatchFilterTournament->SelectedItem->ToString();
            int tournamentId = Int32::Parse(tournamentStr->Substring(0, tournamentStr->IndexOf(L" ")));
            matches = matchRepo->getMatchesByTournament(tournamentId);
        }
        else if (cmbMatchFilterStatus->SelectedIndex > 0)
        {
            matches = matchRepo->getMatchesByStatus(msclr::interop::marshal_as<std::string>(cmbMatchFilterStatus->SelectedItem->ToString()));
        }
        else
        {
            matches = matchRepo->getAllMatches();
        }

        for (int i = 0; i < (int)matches.size(); i++)
        {
            Match m = matches[i];
            matchesGrid->Rows->Add(m.getId(), gcnew String(tournamentRepo->getTournamentDisplayName(m.getTournamentId()).c_str()), gcnew String(teamRepo->getTeamDisplayName(m.getTeam1Id()).c_str()), gcnew String(teamRepo->getTeamDisplayName(m.getTeam2Id()).c_str()), String::Format(L"{0} - {1}", m.getTeam1Score(), m.getTeam2Score()), gcnew String(m.getMatchDate().c_str()), gcnew String(m.getStage().c_str()), gcnew String(m.getStatus().c_str()));
        }
    }

    void LoadMatches()
    {
        LoadFilteredMatches();
    }

    void LoadComboBoxes()
    {
        cmbPlayerTeam->Items->Clear();
        cmbPlayerTeam->Items->Add(L"0 - Free Agent");
        std::vector<Team> teams = teamRepo->getAllTeams();
        for (int i = 0; i < (int)teams.size(); i++)
        {
            Team t = teams[i];
            cmbPlayerTeam->Items->Add(gcnew String((std::to_string(t.getId()) + " - " + t.getName()).c_str()));
        }

        cmbMatchTournament->Items->Clear();
        std::vector<Tournament> tournaments = tournamentRepo->getAllTournaments();
        for (int i = 0; i < (int)tournaments.size(); i++)
        {
            Tournament t = tournaments[i];
            cmbMatchTournament->Items->Add(gcnew String((std::to_string(t.getId()) + " - " + t.getName()).c_str()));
        }
    }

    void OnMatchTournamentChanged(Object^ sender, EventArgs^ e)
    {
        if (cmbMatchTournament->SelectedIndex < 0) return;
        String^ tournamentStr = cmbMatchTournament->SelectedItem->ToString();
        int tournamentId = Int32::Parse(tournamentStr->Substring(0, tournamentStr->IndexOf(L" ")));
        cmbMatchTeam1->Items->Clear();
        cmbMatchTeam2->Items->Clear();
        std::vector<Team> tournamentTeams = teamRepo->getTeamsByTournament(tournamentId);
        for (int i = 0; i < (int)tournamentTeams.size(); i++)
        {
            Team t = tournamentTeams[i];
            String^ teamStr = gcnew String((std::to_string(t.getId()) + " - " + t.getName()).c_str());
            cmbMatchTeam1->Items->Add(teamStr);
            cmbMatchTeam2->Items->Add(teamStr);
        }
    }
    // ========== ВИДЫ СПОРТА ==========
    void InitializeSportsPage()
    {
        Panel^ pagePanel = gcnew Panel();
        pagePanel->Size = contentPanel->Size;
        pagePanel->BackColor = Color::Transparent;
        pagePanel->Tag = (Object^)1;
        pagePanel->Visible = false;

        Panel^ gridPanel = gcnew Panel();
        gridPanel->Size = System::Drawing::Size(contentPanel->Width - 450, pagePanel->Height);
        gridPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ gridTitle = gcnew Label();
        gridTitle->Text = L"\U0001f3ae Виды спорта / Дисциплины";
        gridTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        gridTitle->ForeColor = Color::FromArgb(255, 215, 0);
        gridTitle->Location = Point(20, 15);
        gridTitle->AutoSize = true;

        sportsGrid = gcnew DarkDataGridView();
        sportsGrid->Location = Point(20, 50);
        sportsGrid->Size = System::Drawing::Size(gridPanel->Width - 40, gridPanel->Height - 70);
        sportsGrid->CellClick += gcnew DataGridViewCellEventHandler(this, &MainForm::HandleSportCellClick);

        gridPanel->Controls->Add(gridTitle);
        gridPanel->Controls->Add(sportsGrid);

        Panel^ formPanel = gcnew Panel();
        formPanel->Size = System::Drawing::Size(410, pagePanel->Height);
        formPanel->Location = Point(gridPanel->Width + 20, 0);
        formPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ formTitle = gcnew Label();
        formTitle->Text = L"Информация о дисциплине";
        formTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        formTitle->ForeColor = Color::FromArgb(255, 215, 0);
        formTitle->Location = Point(20, 15);
        formTitle->AutoSize = true;

        int yPos = 60;

        Label^ lblId = CreateFormLabel(L"ID", yPos);
        txtSportId = CreateFormTextBox(true);
        txtSportId->Location = Point(20, yPos + 22);
        txtSportId->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblName = CreateFormLabel(L"Название *", yPos);
        txtSportName = CreateFormTextBox(false);
        txtSportName->Location = Point(20, yPos + 22);
        txtSportName->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblIcon = CreateFormLabel(L"Иконка (эмодзи)", yPos);
        txtSportIcon = CreateFormTextBox(false);
        txtSportIcon->Location = Point(20, yPos + 22);
        txtSportIcon->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblColor = CreateFormLabel(L"Цвет", yPos);
        cmbSportColor = gcnew CyberComboBox();
        cmbSportColor->Location = Point(20, yPos + 22);
        cmbSportColor->Size = System::Drawing::Size(370, 35);
        cmbSportColor->Items->Add(L"#3B82F6 (Синий)");
        cmbSportColor->Items->Add(L"#10B981 (Зеленый)");
        cmbSportColor->Items->Add(L"#F59E0B (Оранжевый)");
        cmbSportColor->Items->Add(L"#8B5CF6 (Фиолетовый)");
        cmbSportColor->Items->Add(L"#EC4899 (Розовый)");
        cmbSportColor->Items->Add(L"#EF4444 (Красный)");
        cmbSportColor->SelectedIndex = 0;
        yPos += 70;

        Label^ lblDescription = CreateFormLabel(L"Описание", yPos);
        txtSportDescription = CreateFormTextBox(false);
        txtSportDescription->Multiline = true;
        txtSportDescription->Location = Point(20, yPos + 22);
        txtSportDescription->Size = System::Drawing::Size(370, 80);
        yPos += 110;

        btnAddSport = CreateActionButton(L"\u2795 Добавить", Color::FromArgb(59, 130, 246));
        btnAddSport->Location = Point(20, yPos);
        btnAddSport->Size = System::Drawing::Size(370, 40);
        btnAddSport->Click += gcnew EventHandler(this, &MainForm::HandleAddSport);
        yPos += 50;

        btnUpdateSport = CreateActionButton(L"\u270e Сохранить", Color::FromArgb(245, 158, 11));
        btnUpdateSport->Location = Point(20, yPos);
        btnUpdateSport->Size = System::Drawing::Size(180, 40);
        btnUpdateSport->Click += gcnew EventHandler(this, &MainForm::HandleUpdateSport);

        btnDeleteSport = CreateActionButton(L"\u2715 Удалить", Color::FromArgb(239, 68, 68));
        btnDeleteSport->Location = Point(210, yPos);
        btnDeleteSport->Size = System::Drawing::Size(180, 40);
        btnDeleteSport->Click += gcnew EventHandler(this, &MainForm::HandleDeleteSport);

        formPanel->Controls->Add(formTitle);
        formPanel->Controls->Add(lblId);
        formPanel->Controls->Add(txtSportId);
        formPanel->Controls->Add(lblName);
        formPanel->Controls->Add(txtSportName);
        formPanel->Controls->Add(lblIcon);
        formPanel->Controls->Add(txtSportIcon);
        formPanel->Controls->Add(lblColor);
        formPanel->Controls->Add(cmbSportColor);
        formPanel->Controls->Add(lblDescription);
        formPanel->Controls->Add(txtSportDescription);
        formPanel->Controls->Add(btnAddSport);
        formPanel->Controls->Add(btnUpdateSport);
        formPanel->Controls->Add(btnDeleteSport);

        pagePanel->Controls->Add(gridPanel);
        pagePanel->Controls->Add(formPanel);
        contentPanel->Controls->Add(pagePanel);
    }

    // ========== ТУРНИРЫ ==========
    void InitializeTournamentsPage()
    {
        Panel^ pagePanel = gcnew Panel();
        pagePanel->Size = contentPanel->Size;
        pagePanel->BackColor = Color::Transparent;
        pagePanel->Tag = (Object^)2;
        pagePanel->Visible = false;

        Panel^ filterPanel = gcnew Panel();
        filterPanel->Size = System::Drawing::Size(contentPanel->Width - 20, 50);
        filterPanel->BackColor = Color::FromArgb(200, 30, 30, 45);
        filterPanel->Location = Point(0, 0);

        Label^ lblFilter = CreateFormLabel(L"Фильтр по дисциплине:", 10);
        lblFilter->Location = Point(20, 15);

        cmbTournamentFilterSport = gcnew CyberComboBox();
        cmbTournamentFilterSport->Location = Point(200, 10);
        cmbTournamentFilterSport->Size = System::Drawing::Size(300, 30);
        cmbTournamentFilterSport->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnTournamentSportFilterChanged);

        filterPanel->Controls->Add(lblFilter);
        filterPanel->Controls->Add(cmbTournamentFilterSport);

        Panel^ gridPanel = gcnew Panel();
        gridPanel->Location = Point(0, 60);
        gridPanel->Size = System::Drawing::Size(contentPanel->Width - 450, pagePanel->Height - 70);
        gridPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ gridTitle = gcnew Label();
        gridTitle->Text = L"\U0001f3c6 Турниры";
        gridTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        gridTitle->ForeColor = Color::FromArgb(255, 215, 0);
        gridTitle->Location = Point(20, 15);
        gridTitle->AutoSize = true;

        tournamentsGrid = gcnew DarkDataGridView();
        tournamentsGrid->Location = Point(20, 50);
        tournamentsGrid->Size = System::Drawing::Size(gridPanel->Width - 40, gridPanel->Height - 110);
        tournamentsGrid->CellClick += gcnew DataGridViewCellEventHandler(this, &MainForm::HandleTournamentCellClick);

        btnViewTournamentDetails = CreateActionButton(L"\U0001f4cb Подробнее о турнире", Color::FromArgb(59, 130, 246));
        btnViewTournamentDetails->Location = Point(20, gridPanel->Height - 50);
        btnViewTournamentDetails->Size = System::Drawing::Size(gridPanel->Width - 40, 35);
        btnViewTournamentDetails->Click += gcnew EventHandler(this, &MainForm::HandleViewTournamentDetails);

        gridPanel->Controls->Add(gridTitle);
        gridPanel->Controls->Add(tournamentsGrid);
        gridPanel->Controls->Add(btnViewTournamentDetails);

        Panel^ formPanel = gcnew Panel();
        formPanel->Size = System::Drawing::Size(410, pagePanel->Height - 70);
        formPanel->Location = Point(gridPanel->Width + 20, 60);
        formPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ formTitle = gcnew Label();
        formTitle->Text = L"Информация о турнире";
        formTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        formTitle->ForeColor = Color::FromArgb(255, 215, 0);
        formTitle->Location = Point(20, 15);
        formTitle->AutoSize = true;

        int yPos = 60;

        Label^ lblId = CreateFormLabel(L"ID", yPos);
        txtTournamentId = CreateFormTextBox(true);
        txtTournamentId->Location = Point(20, yPos + 22);
        txtTournamentId->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblSport = CreateFormLabel(L"Дисциплина *", yPos);
        CyberComboBox^ cmbTournamentSportForm = gcnew CyberComboBox();
        cmbTournamentSportForm->Name = L"cmbTournamentSportForm";
        cmbTournamentSportForm->Location = Point(20, yPos + 22);
        cmbTournamentSportForm->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblName = CreateFormLabel(L"Название *", yPos);
        txtTournamentName = CreateFormTextBox(false);
        txtTournamentName->Location = Point(20, yPos + 22);
        txtTournamentName->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblStartDate = CreateFormLabel(L"Дата начала", yPos);
        dtpTournamentStart = gcnew DateTimePicker();
        dtpTournamentStart->Location = Point(20, yPos + 22);
        dtpTournamentStart->Size = System::Drawing::Size(370, 30);
        dtpTournamentStart->Format = DateTimePickerFormat::Short;
        dtpTournamentStart->Font = GetEmojiFont(10.0f);
        yPos += 70;

        Label^ lblEndDate = CreateFormLabel(L"Дата окончания", yPos);
        dtpTournamentEnd = gcnew DateTimePicker();
        dtpTournamentEnd->Location = Point(20, yPos + 22);
        dtpTournamentEnd->Size = System::Drawing::Size(370, 30);
        dtpTournamentEnd->Format = DateTimePickerFormat::Short;
        dtpTournamentEnd->Font = GetEmojiFont(10.0f);
        yPos += 70;

        Label^ lblPrize = CreateFormLabel(L"Призовой фонд ($)", yPos);
        txtTournamentPrize = CreateFormTextBox(false);
        txtTournamentPrize->Location = Point(20, yPos + 22);
        txtTournamentPrize->Size = System::Drawing::Size(370, 35);
        txtTournamentPrize->Text = L"0";
        yPos += 70;

        Label^ lblStatus = CreateFormLabel(L"Статус", yPos);
        cmbTournamentStatus = gcnew CyberComboBox();
        cmbTournamentStatus->Location = Point(20, yPos + 22);
        cmbTournamentStatus->Size = System::Drawing::Size(370, 35);
        cmbTournamentStatus->Items->Add(L"upcoming");
        cmbTournamentStatus->Items->Add(L"active");
        cmbTournamentStatus->Items->Add(L"completed");
        cmbTournamentStatus->SelectedIndex = 0;
        yPos += 80;

        btnAddTournament = CreateActionButton(L"\u2795 Добавить", Color::FromArgb(16, 185, 129));
        btnAddTournament->Location = Point(20, yPos);
        btnAddTournament->Size = System::Drawing::Size(370, 35);
        btnAddTournament->Click += gcnew EventHandler(this, &MainForm::HandleAddTournament);
        yPos += 45;

        btnUpdateTournament = CreateActionButton(L"\u270e Сохранить", Color::FromArgb(245, 158, 11));
        btnUpdateTournament->Location = Point(20, yPos);
        btnUpdateTournament->Size = System::Drawing::Size(120, 35);
        btnUpdateTournament->Click += gcnew EventHandler(this, &MainForm::HandleUpdateTournament);

        btnDeleteTournament = CreateActionButton(L"\u2715 Удалить", Color::FromArgb(239, 68, 68));
        btnDeleteTournament->Location = Point(145, yPos);
        btnDeleteTournament->Size = System::Drawing::Size(120, 35);
        btnDeleteTournament->Click += gcnew EventHandler(this, &MainForm::HandleDeleteTournament);

        btnStartTournament = CreateActionButton(L"\u25b6 Старт", Color::FromArgb(59, 130, 246));
        btnStartTournament->Location = Point(270, yPos);
        btnStartTournament->Size = System::Drawing::Size(120, 35);
        btnStartTournament->Click += gcnew EventHandler(this, &MainForm::HandleStartTournament);
        yPos += 45;

        btnManageTournamentTeams = CreateActionButton(L"\U0001f465 Команды турнира", Color::FromArgb(139, 92, 246));
        btnManageTournamentTeams->Location = Point(20, yPos);
        btnManageTournamentTeams->Size = System::Drawing::Size(370, 35);
        btnManageTournamentTeams->Click += gcnew EventHandler(this, &MainForm::HandleManageTournamentTeams);

        formPanel->Controls->Add(formTitle);
        formPanel->Controls->Add(lblId);
        formPanel->Controls->Add(txtTournamentId);
        formPanel->Controls->Add(lblSport);
        formPanel->Controls->Add(cmbTournamentSportForm);
        formPanel->Controls->Add(lblName);
        formPanel->Controls->Add(txtTournamentName);
        formPanel->Controls->Add(lblStartDate);
        formPanel->Controls->Add(dtpTournamentStart);
        formPanel->Controls->Add(lblEndDate);
        formPanel->Controls->Add(dtpTournamentEnd);
        formPanel->Controls->Add(lblPrize);
        formPanel->Controls->Add(txtTournamentPrize);
        formPanel->Controls->Add(lblStatus);
        formPanel->Controls->Add(cmbTournamentStatus);
        formPanel->Controls->Add(btnAddTournament);
        formPanel->Controls->Add(btnUpdateTournament);
        formPanel->Controls->Add(btnDeleteTournament);
        formPanel->Controls->Add(btnStartTournament);
        formPanel->Controls->Add(btnManageTournamentTeams);

        pagePanel->Controls->Add(filterPanel);
        pagePanel->Controls->Add(gridPanel);
        pagePanel->Controls->Add(formPanel);
        contentPanel->Controls->Add(pagePanel);
    }

    // ========== ДЕТАЛИ ТУРНИРА ==========
    void InitializeTournamentDetailPage()
    {
        tournamentDetailPanel = gcnew Panel();
        tournamentDetailPanel->Size = contentPanel->Size;
        tournamentDetailPanel->BackColor = Color::Transparent;
        tournamentDetailPanel->Tag = (Object^)10;
        tournamentDetailPanel->Visible = false;
        tournamentDetailPanel->AutoScroll = true;

        btnBackToTournaments = CreateActionButton(L"\u2190 Назад к списку турниров", Color::FromArgb(100, 100, 120));
        btnBackToTournaments->Location = Point(0, 0);
        btnBackToTournaments->Size = System::Drawing::Size(250, 35);
        btnBackToTournaments->Click += gcnew EventHandler(this, &MainForm::HandleBackToTournaments);

        lblTournamentDetailTitle = gcnew Label();
        lblTournamentDetailTitle->Text = L"Детали турнира";
        lblTournamentDetailTitle->Font = GetEmojiFont(18.0f, FontStyle::Bold);
        lblTournamentDetailTitle->ForeColor = Color::FromArgb(255, 215, 0);
        lblTournamentDetailTitle->Location = Point(0, 45);
        lblTournamentDetailTitle->AutoSize = true;

        Label^ lblTeamsTitle = gcnew Label();
        lblTeamsTitle->Text = L"\U0001f465 Команды-участники";
        lblTeamsTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        lblTeamsTitle->ForeColor = Color::FromArgb(255, 215, 0);
        lblTeamsTitle->Location = Point(0, 85);

        tournamentTeamsGrid = gcnew DarkDataGridView();
        tournamentTeamsGrid->Location = Point(0, 115);
        tournamentTeamsGrid->Size = System::Drawing::Size(contentPanel->Width - 20, 250);

        Label^ lblMatchesTitle = gcnew Label();
        lblMatchesTitle->Text = L"\u2694\ufe0f Матчи турнира";
        lblMatchesTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        lblMatchesTitle->ForeColor = Color::FromArgb(255, 215, 0);
        lblMatchesTitle->Location = Point(0, 380);

        tournamentMatchesGrid = gcnew DarkDataGridView();
        tournamentMatchesGrid->Location = Point(0, 410);
        tournamentMatchesGrid->Size = System::Drawing::Size(contentPanel->Width - 20, 300);

        tournamentDetailPanel->Controls->Add(btnBackToTournaments);
        tournamentDetailPanel->Controls->Add(lblTournamentDetailTitle);
        tournamentDetailPanel->Controls->Add(lblTeamsTitle);
        tournamentDetailPanel->Controls->Add(tournamentTeamsGrid);
        tournamentDetailPanel->Controls->Add(lblMatchesTitle);
        tournamentDetailPanel->Controls->Add(tournamentMatchesGrid);

        contentPanel->Controls->Add(tournamentDetailPanel);
    }
    // ========== КОМАНДЫ ==========
    void InitializeTeamsPage()
    {
        Panel^ pagePanel = gcnew Panel();
        pagePanel->Size = contentPanel->Size;
        pagePanel->BackColor = Color::Transparent;
        pagePanel->Tag = (Object^)3;
        pagePanel->Visible = false;

        Panel^ filterPanel = gcnew Panel();
        filterPanel->Size = System::Drawing::Size(contentPanel->Width - 20, 50);
        filterPanel->BackColor = Color::FromArgb(200, 30, 30, 45);
        filterPanel->Location = Point(0, 0);

        Label^ lblFilter = CreateFormLabel(L"Фильтр по дисциплине:", 10);
        lblFilter->Location = Point(20, 15);

        cmbTeamFilterSport = gcnew CyberComboBox();
        cmbTeamFilterSport->Location = Point(200, 10);
        cmbTeamFilterSport->Size = System::Drawing::Size(300, 30);
        cmbTeamFilterSport->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnTeamSportFilterChanged);

        filterPanel->Controls->Add(lblFilter);
        filterPanel->Controls->Add(cmbTeamFilterSport);

        Panel^ gridPanel = gcnew Panel();
        gridPanel->Location = Point(0, 60);
        gridPanel->Size = System::Drawing::Size(contentPanel->Width - 450, pagePanel->Height - 70);
        gridPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ gridTitle = gcnew Label();
        gridTitle->Text = L"\U0001f465 Команды";
        gridTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        gridTitle->ForeColor = Color::FromArgb(255, 215, 0);
        gridTitle->Location = Point(20, 15);
        gridTitle->AutoSize = true;

        teamsGrid = gcnew DarkDataGridView();
        teamsGrid->Location = Point(20, 50);
        teamsGrid->Size = System::Drawing::Size(gridPanel->Width - 40, gridPanel->Height - 110);
        teamsGrid->CellClick += gcnew DataGridViewCellEventHandler(this, &MainForm::HandleTeamCellClick);

        btnViewTeamDetails = CreateActionButton(L"\U0001f4cb Подробнее о команде", Color::FromArgb(59, 130, 246));
        btnViewTeamDetails->Location = Point(20, gridPanel->Height - 50);
        btnViewTeamDetails->Size = System::Drawing::Size(gridPanel->Width - 40, 35);
        btnViewTeamDetails->Click += gcnew EventHandler(this, &MainForm::HandleViewTeamDetails);

        gridPanel->Controls->Add(gridTitle);
        gridPanel->Controls->Add(teamsGrid);
        gridPanel->Controls->Add(btnViewTeamDetails);

        Panel^ formPanel = gcnew Panel();
        formPanel->Size = System::Drawing::Size(410, pagePanel->Height - 70);
        formPanel->Location = Point(gridPanel->Width + 20, 60);
        formPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ formTitle = gcnew Label();
        formTitle->Text = L"Информация о команде";
        formTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        formTitle->ForeColor = Color::FromArgb(255, 215, 0);
        formTitle->Location = Point(20, 15);
        formTitle->AutoSize = true;

        int yPos = 60;

        Label^ lblId = CreateFormLabel(L"ID", yPos);
        txtTeamId = CreateFormTextBox(true);
        txtTeamId->Location = Point(20, yPos + 22);
        txtTeamId->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblSport = CreateFormLabel(L"Дисциплина *", yPos);
        CyberComboBox^ cmbTeamSportForm = gcnew CyberComboBox();
        cmbTeamSportForm->Name = L"cmbTeamSportForm";
        cmbTeamSportForm->Location = Point(20, yPos + 22);
        cmbTeamSportForm->Size = System::Drawing::Size(370, 35);

        formPanel->Controls->Add(lblSport);
        formPanel->Controls->Add(cmbTeamSportForm);
        yPos += 70;

        Label^ lblName = CreateFormLabel(L"Название *", yPos);
        txtTeamName = CreateFormTextBox(false);
        txtTeamName->Location = Point(20, yPos + 22);
        txtTeamName->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblTag = CreateFormLabel(L"Тег [TAG]", yPos);
        txtTeamTag = CreateFormTextBox(false);
        txtTeamTag->Location = Point(20, yPos + 22);
        txtTeamTag->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblLogo = CreateFormLabel(L"Лого (эмодзи)", yPos);
        txtTeamLogo = CreateFormTextBox(false);
        txtTeamLogo->Location = Point(20, yPos + 22);
        txtTeamLogo->Size = System::Drawing::Size(370, 35);
        txtTeamLogo->Text = L"\U0001f3e2";
        yPos += 80;

        btnAddTeam = CreateActionButton(L"\u2795 Добавить", Color::FromArgb(245, 158, 11));
        btnAddTeam->Location = Point(20, yPos);
        btnAddTeam->Size = System::Drawing::Size(370, 40);
        btnAddTeam->Click += gcnew EventHandler(this, &MainForm::HandleAddTeam);
        yPos += 50;

        btnUpdateTeam = CreateActionButton(L"\u270e Сохранить", Color::FromArgb(139, 92, 246));
        btnUpdateTeam->Location = Point(20, yPos);
        btnUpdateTeam->Size = System::Drawing::Size(180, 40);
        btnUpdateTeam->Click += gcnew EventHandler(this, &MainForm::HandleUpdateTeam);

        btnDeleteTeam = CreateActionButton(L"\u2715 Удалить", Color::FromArgb(239, 68, 68));
        btnDeleteTeam->Location = Point(210, yPos);
        btnDeleteTeam->Size = System::Drawing::Size(180, 40);
        btnDeleteTeam->Click += gcnew EventHandler(this, &MainForm::HandleDeleteTeam);

        formPanel->Controls->Add(formTitle);
        formPanel->Controls->Add(lblId);
        formPanel->Controls->Add(txtTeamId);
        formPanel->Controls->Add(lblName);
        formPanel->Controls->Add(txtTeamName);
        formPanel->Controls->Add(lblTag);
        formPanel->Controls->Add(txtTeamTag);
        formPanel->Controls->Add(lblLogo);
        formPanel->Controls->Add(txtTeamLogo);
        formPanel->Controls->Add(btnAddTeam);
        formPanel->Controls->Add(btnUpdateTeam);
        formPanel->Controls->Add(btnDeleteTeam);

        pagePanel->Controls->Add(filterPanel);
        pagePanel->Controls->Add(gridPanel);
        pagePanel->Controls->Add(formPanel);
        contentPanel->Controls->Add(pagePanel);
    }

    // ========== ДЕТАЛИ КОМАНДЫ ==========
    void InitializeTeamDetailPage()
    {
        teamDetailPanel = gcnew Panel();
        teamDetailPanel->Size = contentPanel->Size;
        teamDetailPanel->BackColor = Color::Transparent;
        teamDetailPanel->Tag = (Object^)11;
        teamDetailPanel->Visible = false;
        teamDetailPanel->AutoScroll = true;

        btnBackToTeams = CreateActionButton(L"\u2190 Назад к списку команд", Color::FromArgb(100, 100, 120));
        btnBackToTeams->Location = Point(0, 0);
        btnBackToTeams->Size = System::Drawing::Size(250, 35);
        btnBackToTeams->Click += gcnew EventHandler(this, &MainForm::HandleBackToTeams);

        lblTeamDetailTitle = gcnew Label();
        lblTeamDetailTitle->Text = L"Детали команды";
        lblTeamDetailTitle->Font = GetEmojiFont(18.0f, FontStyle::Bold);
        lblTeamDetailTitle->ForeColor = Color::FromArgb(255, 215, 0);
        lblTeamDetailTitle->Location = Point(0, 45);
        lblTeamDetailTitle->AutoSize = true;

        Label^ lblPlayersTitle = gcnew Label();
        lblPlayersTitle->Text = L"\U0001f3af Игроки команды";
        lblPlayersTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        lblPlayersTitle->ForeColor = Color::FromArgb(255, 215, 0);
        lblPlayersTitle->Location = Point(0, 85);

        teamPlayersGrid = gcnew DarkDataGridView();
        teamPlayersGrid->Location = Point(0, 115);
        teamPlayersGrid->Size = System::Drawing::Size(contentPanel->Width - 20, 200);

        Label^ lblHistoryTitle = gcnew Label();
        lblHistoryTitle->Text = L"\U0001f4dc История матчей";
        lblHistoryTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        lblHistoryTitle->ForeColor = Color::FromArgb(255, 215, 0);
        lblHistoryTitle->Location = Point(0, 330);

        teamMatchesHistoryGrid = gcnew DarkDataGridView();
        teamMatchesHistoryGrid->Location = Point(0, 360);
        teamMatchesHistoryGrid->Size = System::Drawing::Size(contentPanel->Width - 20, 200);

        Label^ lblUpcomingTitle = gcnew Label();
        lblUpcomingTitle->Text = L"\U0001f4c5 Запланированные матчи";
        lblUpcomingTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        lblUpcomingTitle->ForeColor = Color::FromArgb(255, 215, 0);
        lblUpcomingTitle->Location = Point(0, 575);

        teamUpcomingMatchesGrid = gcnew DarkDataGridView();
        teamUpcomingMatchesGrid->Location = Point(0, 605);
        teamUpcomingMatchesGrid->Size = System::Drawing::Size(contentPanel->Width - 20, 200);

        teamDetailPanel->Controls->Add(btnBackToTeams);
        teamDetailPanel->Controls->Add(lblTeamDetailTitle);
        teamDetailPanel->Controls->Add(lblPlayersTitle);
        teamDetailPanel->Controls->Add(teamPlayersGrid);
        teamDetailPanel->Controls->Add(lblHistoryTitle);
        teamDetailPanel->Controls->Add(teamMatchesHistoryGrid);
        teamDetailPanel->Controls->Add(lblUpcomingTitle);
        teamDetailPanel->Controls->Add(teamUpcomingMatchesGrid);

        contentPanel->Controls->Add(teamDetailPanel);
    }

    // ========== ИГРОКИ ==========
    void InitializePlayersPage()
    {
        Panel^ pagePanel = gcnew Panel();
        pagePanel->Size = contentPanel->Size;
        pagePanel->BackColor = Color::Transparent;
        pagePanel->Tag = (Object^)4;
        pagePanel->Visible = false;

        Panel^ gridPanel = gcnew Panel();
        gridPanel->Size = System::Drawing::Size(contentPanel->Width - 450, pagePanel->Height);
        gridPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ gridTitle = gcnew Label();
        gridTitle->Text = L"\U0001f3af Игроки";
        gridTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        gridTitle->ForeColor = Color::FromArgb(255, 215, 0);
        gridTitle->Location = Point(20, 15);
        gridTitle->AutoSize = true;

        playersGrid = gcnew DarkDataGridView();
        playersGrid->Location = Point(20, 50);
        playersGrid->Size = System::Drawing::Size(gridPanel->Width - 40, gridPanel->Height - 70);
        playersGrid->CellClick += gcnew DataGridViewCellEventHandler(this, &MainForm::HandlePlayerCellClick);

        gridPanel->Controls->Add(gridTitle);
        gridPanel->Controls->Add(playersGrid);

        Panel^ formPanel = gcnew Panel();
        formPanel->Size = System::Drawing::Size(410, pagePanel->Height);
        formPanel->Location = Point(gridPanel->Width + 20, 0);
        formPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ formTitle = gcnew Label();
        formTitle->Text = L"Информация об игроке";
        formTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        formTitle->ForeColor = Color::FromArgb(255, 215, 0);
        formTitle->Location = Point(20, 15);
        formTitle->AutoSize = true;

        int yPos = 60;

        Label^ lblId = CreateFormLabel(L"ID", yPos);
        txtPlayerId = CreateFormTextBox(true);
        txtPlayerId->Location = Point(20, yPos + 22);
        txtPlayerId->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblTeam = CreateFormLabel(L"Команда", yPos);
        cmbPlayerTeam = gcnew CyberComboBox();
        cmbPlayerTeam->Location = Point(20, yPos + 22);
        cmbPlayerTeam->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblNickname = CreateFormLabel(L"Никнейм *", yPos);
        txtPlayerNickname = CreateFormTextBox(false);
        txtPlayerNickname->Location = Point(20, yPos + 22);
        txtPlayerNickname->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblFullName = CreateFormLabel(L"Полное имя", yPos);
        txtPlayerFullName = CreateFormTextBox(false);
        txtPlayerFullName->Location = Point(20, yPos + 22);
        txtPlayerFullName->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblRole = CreateFormLabel(L"Роль", yPos);
        txtPlayerRole = CreateFormTextBox(false);
        txtPlayerRole->Location = Point(20, yPos + 22);
        txtPlayerRole->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblNationality = CreateFormLabel(L"Национальность", yPos);
        txtPlayerNationality = CreateFormTextBox(false);
        txtPlayerNationality->Location = Point(20, yPos + 22);
        txtPlayerNationality->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblRating = CreateFormLabel(L"Рейтинг", yPos);
        txtPlayerRating = CreateFormTextBox(false);
        txtPlayerRating->Location = Point(20, yPos + 22);
        txtPlayerRating->Size = System::Drawing::Size(370, 35);
        txtPlayerRating->Text = L"1000";
        yPos += 80;

        btnAddPlayer = CreateActionButton(L"\u2795 Добавить", Color::FromArgb(139, 92, 246));
        btnAddPlayer->Location = Point(20, yPos);
        btnAddPlayer->Size = System::Drawing::Size(370, 40);
        btnAddPlayer->Click += gcnew EventHandler(this, &MainForm::HandleAddPlayer);
        yPos += 50;

        btnUpdatePlayer = CreateActionButton(L"\u270e Сохранить", Color::FromArgb(236, 72, 153));
        btnUpdatePlayer->Location = Point(20, yPos);
        btnUpdatePlayer->Size = System::Drawing::Size(180, 40);
        btnUpdatePlayer->Click += gcnew EventHandler(this, &MainForm::HandleUpdatePlayer);

        btnDeletePlayer = CreateActionButton(L"\u2715 Удалить", Color::FromArgb(239, 68, 68));
        btnDeletePlayer->Location = Point(210, yPos);
        btnDeletePlayer->Size = System::Drawing::Size(180, 40);
        btnDeletePlayer->Click += gcnew EventHandler(this, &MainForm::HandleDeletePlayer);

        formPanel->Controls->Add(formTitle);
        formPanel->Controls->Add(lblId);
        formPanel->Controls->Add(txtPlayerId);
        formPanel->Controls->Add(lblTeam);
        formPanel->Controls->Add(cmbPlayerTeam);
        formPanel->Controls->Add(lblNickname);
        formPanel->Controls->Add(txtPlayerNickname);
        formPanel->Controls->Add(lblFullName);
        formPanel->Controls->Add(txtPlayerFullName);
        formPanel->Controls->Add(lblRole);
        formPanel->Controls->Add(txtPlayerRole);
        formPanel->Controls->Add(lblNationality);
        formPanel->Controls->Add(txtPlayerNationality);
        formPanel->Controls->Add(lblRating);
        formPanel->Controls->Add(txtPlayerRating);
        formPanel->Controls->Add(btnAddPlayer);
        formPanel->Controls->Add(btnUpdatePlayer);
        formPanel->Controls->Add(btnDeletePlayer);

        pagePanel->Controls->Add(gridPanel);
        pagePanel->Controls->Add(formPanel);
        contentPanel->Controls->Add(pagePanel);
    }

    // ========== МАТЧИ ==========
    void InitializeMatchesPage()
    {
        Panel^ pagePanel = gcnew Panel();
        pagePanel->Size = contentPanel->Size;
        pagePanel->BackColor = Color::Transparent;
        pagePanel->Tag = (Object^)5;
        pagePanel->Visible = false;

        // Панель фильтров матчей
        Panel^ filterPanel = gcnew Panel();
        filterPanel->Size = System::Drawing::Size(contentPanel->Width - 20, 50);
        filterPanel->BackColor = Color::FromArgb(200, 30, 30, 45);
        filterPanel->Location = Point(0, 0);

        Label^ lblFilterTournament = CreateFormLabel(L"Турнир:", 5);
        lblFilterTournament->Location = Point(20, 15);

        cmbMatchFilterTournament = gcnew CyberComboBox();
        cmbMatchFilterTournament->Location = Point(100, 10);
        cmbMatchFilterTournament->Size = System::Drawing::Size(250, 30);
        cmbMatchFilterTournament->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnMatchFilterChanged);

        Label^ lblFilterStatus = CreateFormLabel(L"Статус:", 5);
        lblFilterStatus->Location = Point(370, 15);

        cmbMatchFilterStatus = gcnew CyberComboBox();
        cmbMatchFilterStatus->Location = Point(440, 10);
        cmbMatchFilterStatus->Size = System::Drawing::Size(150, 30);
        cmbMatchFilterStatus->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnMatchFilterChanged);

        CyberButton^ btnResetMatchFilters = CreateActionButton(L"Сбросить", Color::FromArgb(100, 100, 120));
        btnResetMatchFilters->Location = Point(610, 10);
        btnResetMatchFilters->Size = System::Drawing::Size(100, 30);
        btnResetMatchFilters->Click += gcnew EventHandler(this, &MainForm::OnResetMatchFilters);

        filterPanel->Controls->Add(lblFilterTournament);
        filterPanel->Controls->Add(cmbMatchFilterTournament);
        filterPanel->Controls->Add(lblFilterStatus);
        filterPanel->Controls->Add(cmbMatchFilterStatus);
        filterPanel->Controls->Add(btnResetMatchFilters);

        // Панель таблицы матчей
        Panel^ gridPanel = gcnew Panel();
        gridPanel->Location = Point(0, 60);
        gridPanel->Size = System::Drawing::Size(contentPanel->Width - 450, pagePanel->Height - 70);
        gridPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ gridTitle = gcnew Label();
        gridTitle->Text = L"\u2694\ufe0f Матчи";
        gridTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        gridTitle->ForeColor = Color::FromArgb(255, 215, 0);
        gridTitle->Location = Point(20, 15);
        gridTitle->AutoSize = true;

        matchesGrid = gcnew DarkDataGridView();
        matchesGrid->Location = Point(20, 50);
        matchesGrid->Size = System::Drawing::Size(gridPanel->Width - 40, gridPanel->Height - 70);
        matchesGrid->CellClick += gcnew DataGridViewCellEventHandler(this, &MainForm::HandleMatchCellClick);

        gridPanel->Controls->Add(gridTitle);
        gridPanel->Controls->Add(matchesGrid);

        // Панель формы матча
        Panel^ formPanel = gcnew Panel();
        formPanel->Size = System::Drawing::Size(410, pagePanel->Height - 70);
        formPanel->Location = Point(gridPanel->Width + 20, 60);
        formPanel->BackColor = Color::FromArgb(200, 30, 30, 45);

        Label^ formTitle = gcnew Label();
        formTitle->Text = L"Информация о матче";
        formTitle->Font = GetEmojiFont(14.0f, FontStyle::Bold);
        formTitle->ForeColor = Color::FromArgb(255, 215, 0);
        formTitle->Location = Point(20, 15);
        formTitle->AutoSize = true;

        int yPos = 60;

        Label^ lblId = CreateFormLabel(L"ID", yPos);
        txtMatchId = CreateFormTextBox(true);
        txtMatchId->Location = Point(20, yPos + 22);
        txtMatchId->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblTournament = CreateFormLabel(L"Турнир *", yPos);
        cmbMatchTournament = gcnew CyberComboBox();
        cmbMatchTournament->Location = Point(20, yPos + 22);
        cmbMatchTournament->Size = System::Drawing::Size(370, 35);
        cmbMatchTournament->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnMatchTournamentChanged);
        yPos += 70;

        Label^ lblTeam1 = CreateFormLabel(L"Команда 1 *", yPos);
        cmbMatchTeam1 = gcnew CyberComboBox();
        cmbMatchTeam1->Location = Point(20, yPos + 22);
        cmbMatchTeam1->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblTeam2 = CreateFormLabel(L"Команда 2 *", yPos);
        cmbMatchTeam2 = gcnew CyberComboBox();
        cmbMatchTeam2->Location = Point(20, yPos + 22);
        cmbMatchTeam2->Size = System::Drawing::Size(370, 35);
        yPos += 70;

        Label^ lblMatchDate = CreateFormLabel(L"Дата матча", yPos);
        dtpMatchDate = gcnew DateTimePicker();
        dtpMatchDate->Location = Point(20, yPos + 22);
        dtpMatchDate->Size = System::Drawing::Size(370, 30);
        dtpMatchDate->Format = DateTimePickerFormat::Custom;
        dtpMatchDate->CustomFormat = L"dd MMMM yyyy HH:mm";
        dtpMatchDate->Font = GetEmojiFont(10.0f);
        yPos += 70;

        Label^ lblScore = CreateFormLabel(L"Счет", yPos);
        Panel^ scorePanel = gcnew Panel();
        scorePanel->Location = Point(20, yPos + 22);
        scorePanel->Size = System::Drawing::Size(370, 35);
        scorePanel->BackColor = Color::Transparent;

        txtMatchScore1 = CreateFormTextBox(false);
        txtMatchScore1->Location = Point(0, 0);
        txtMatchScore1->Size = System::Drawing::Size(80, 35);
        txtMatchScore1->Text = L"0";

        Label^ vsLabel = gcnew Label();
        vsLabel->Text = L"VS";
        vsLabel->Font = GetEmojiFont(12.0f, FontStyle::Bold);
        vsLabel->ForeColor = Color::FromArgb(255, 215, 0);
        vsLabel->Location = Point(95, 5);
        vsLabel->AutoSize = true;

        txtMatchScore2 = CreateFormTextBox(false);
        txtMatchScore2->Location = Point(140, 0);
        txtMatchScore2->Size = System::Drawing::Size(80, 35);
        txtMatchScore2->Text = L"0";

        scorePanel->Controls->Add(txtMatchScore1);
        scorePanel->Controls->Add(vsLabel);
        scorePanel->Controls->Add(txtMatchScore2);
        yPos += 70;

        Label^ lblStage = CreateFormLabel(L"Стадия", yPos);
        cmbMatchStage = gcnew CyberComboBox();
        cmbMatchStage->Location = Point(20, yPos + 22);
        cmbMatchStage->Size = System::Drawing::Size(370, 35);
        cmbMatchStage->Items->Add(L"group");
        cmbMatchStage->Items->Add(L"quarterfinal");
        cmbMatchStage->Items->Add(L"semifinal");
        cmbMatchStage->Items->Add(L"final");
        cmbMatchStage->SelectedIndex = 0;
        yPos += 70;

        Label^ lblStatus = CreateFormLabel(L"Статус", yPos);
        cmbMatchStatus = gcnew CyberComboBox();
        cmbMatchStatus->Location = Point(20, yPos + 22);
        cmbMatchStatus->Size = System::Drawing::Size(370, 35);
        cmbMatchStatus->Items->Add(L"scheduled");
        cmbMatchStatus->Items->Add(L"live");
        cmbMatchStatus->Items->Add(L"completed");
        cmbMatchStatus->SelectedIndex = 0;
        yPos += 80;

        btnAddMatch = CreateActionButton(L"\u2795 Добавить", Color::FromArgb(236, 72, 153));
        btnAddMatch->Location = Point(20, yPos);
        btnAddMatch->Size = System::Drawing::Size(370, 35);
        btnAddMatch->Click += gcnew EventHandler(this, &MainForm::HandleAddMatch);
        yPos += 45;

        btnUpdateMatch = CreateActionButton(L"\u270e Сохранить", Color::FromArgb(245, 158, 11));
        btnUpdateMatch->Location = Point(20, yPos);
        btnUpdateMatch->Size = System::Drawing::Size(90, 35);
        btnUpdateMatch->Click += gcnew EventHandler(this, &MainForm::HandleUpdateMatch);

        btnDeleteMatch = CreateActionButton(L"\u2715 Удалить", Color::FromArgb(239, 68, 68));
        btnDeleteMatch->Location = Point(115, yPos);
        btnDeleteMatch->Size = System::Drawing::Size(90, 35);
        btnDeleteMatch->Click += gcnew EventHandler(this, &MainForm::HandleDeleteMatch);

        btnStartMatch = CreateActionButton(L"\u25b6 Старт", Color::FromArgb(59, 130, 246));
        btnStartMatch->Location = Point(210, yPos);
        btnStartMatch->Size = System::Drawing::Size(85, 35);
        btnStartMatch->Click += gcnew EventHandler(this, &MainForm::HandleStartMatch);

        btnFinishMatch = CreateActionButton(L"\U0001f3c1 Финиш", Color::FromArgb(16, 185, 129));
        btnFinishMatch->Location = Point(300, yPos);
        btnFinishMatch->Size = System::Drawing::Size(90, 35);
        btnFinishMatch->Click += gcnew EventHandler(this, &MainForm::HandleFinishMatch);

        formPanel->Controls->Add(formTitle);
        formPanel->Controls->Add(lblId);
        formPanel->Controls->Add(txtMatchId);
        formPanel->Controls->Add(lblTournament);
        formPanel->Controls->Add(cmbMatchTournament);
        formPanel->Controls->Add(lblTeam1);
        formPanel->Controls->Add(cmbMatchTeam1);
        formPanel->Controls->Add(lblTeam2);
        formPanel->Controls->Add(cmbMatchTeam2);
        formPanel->Controls->Add(lblMatchDate);
        formPanel->Controls->Add(dtpMatchDate);
        formPanel->Controls->Add(lblScore);
        formPanel->Controls->Add(scorePanel);
        formPanel->Controls->Add(lblStage);
        formPanel->Controls->Add(cmbMatchStage);
        formPanel->Controls->Add(lblStatus);
        formPanel->Controls->Add(cmbMatchStatus);
        formPanel->Controls->Add(btnAddMatch);
        formPanel->Controls->Add(btnUpdateMatch);
        formPanel->Controls->Add(btnDeleteMatch);
        formPanel->Controls->Add(btnStartMatch);
        formPanel->Controls->Add(btnFinishMatch);

        pagePanel->Controls->Add(filterPanel);
        pagePanel->Controls->Add(gridPanel);
        pagePanel->Controls->Add(formPanel);
        contentPanel->Controls->Add(pagePanel);
    }
    // ========== ДЕТАЛИ ТУРНИРА (методы) ==========
    void HandleViewTournamentDetails(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTournamentId->Text))
        {
            MessageBox::Show(L"Выберите турнир для просмотра!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        selectedTournamentId = Int32::Parse(txtTournamentId->Text);
        for each (Control ^ ctrl in contentPanel->Controls) { if (ctrl->Tag != nullptr) ctrl->Visible = false; }
        tournamentDetailPanel->Visible = true;
        pageTitle->Text = L"\U0001f4cb Детали турнира";
        LoadTournamentDetails();
    }

    void LoadTournamentDetails()
    {
        Tournament t = tournamentRepo->getTournamentById(selectedTournamentId);
        lblTournamentDetailTitle->Text = String::Format(L"\U0001f3c6 {0}", gcnew String(t.getName().c_str()));

        tournamentTeamsGrid->Rows->Clear();
        if (tournamentTeamsGrid->Columns->Count == 0)
        {
            tournamentTeamsGrid->Columns->Add(L"ID", L"ID");
            tournamentTeamsGrid->Columns->Add(L"Name", L"Команда");
            tournamentTeamsGrid->Columns->Add(L"Tag", L"Тег");
            tournamentTeamsGrid->Columns->Add(L"W", L"W");
            tournamentTeamsGrid->Columns->Add(L"L", L"L");
            tournamentTeamsGrid->Columns->Add(L"Pts", L"Очки");
        }
        std::vector<Team> teams = teamRepo->getTeamsByTournament(selectedTournamentId);
        for (int i = 0; i < (int)teams.size(); i++)
        {
            Team team = teams[i];
            tournamentTeamsGrid->Rows->Add(team.getId(), gcnew String(team.getName().c_str()), gcnew String(team.getTag().c_str()), team.getWins(), team.getLosses(), team.getPoints());
        }

        tournamentMatchesGrid->Rows->Clear();
        if (tournamentMatchesGrid->Columns->Count == 0)
        {
            tournamentMatchesGrid->Columns->Add(L"ID", L"ID");
            tournamentMatchesGrid->Columns->Add(L"Team 1", L"Команда 1");
            tournamentMatchesGrid->Columns->Add(L"Team 2", L"Команда 2");
            tournamentMatchesGrid->Columns->Add(L"Score", L"Счет");
            tournamentMatchesGrid->Columns->Add(L"Date", L"Дата");
            tournamentMatchesGrid->Columns->Add(L"Stage", L"Стадия");
            tournamentMatchesGrid->Columns->Add(L"Status", L"Статус");
        }
        std::vector<Match> matches = matchRepo->getMatchesByTournament(selectedTournamentId);
        for (int i = 0; i < (int)matches.size(); i++)
        {
            Match m = matches[i];
            tournamentMatchesGrid->Rows->Add(m.getId(), gcnew String(teamRepo->getTeamDisplayName(m.getTeam1Id()).c_str()), gcnew String(teamRepo->getTeamDisplayName(m.getTeam2Id()).c_str()), String::Format(L"{0} - {1}", m.getTeam1Score(), m.getTeam2Score()), gcnew String(m.getMatchDate().c_str()), gcnew String(m.getStage().c_str()), gcnew String(m.getStatus().c_str()));
        }
    }

    void HandleBackToTournaments(Object^ sender, EventArgs^ e)
    {
        tournamentDetailPanel->Visible = false;
        SwitchPage(2);
    }

    // ========== ДЕТАЛИ КОМАНДЫ (методы) ==========
    void HandleViewTeamDetails(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTeamId->Text))
        {
            MessageBox::Show(L"Выберите команду для просмотра!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        selectedTeamId = Int32::Parse(txtTeamId->Text);
        for each (Control ^ ctrl in contentPanel->Controls) { if (ctrl->Tag != nullptr) ctrl->Visible = false; }
        teamDetailPanel->Visible = true;
        pageTitle->Text = L"\U0001f4cb Детали команды";
        LoadTeamDetails();
    }

    void LoadTeamDetails()
    {
        Team team = teamRepo->getTeamById(selectedTeamId);
        lblTeamDetailTitle->Text = String::Format(L"\U0001f465 {0} [{1}]", gcnew String(team.getName().c_str()), gcnew String(team.getTag().c_str()));

        teamPlayersGrid->Rows->Clear();
        if (teamPlayersGrid->Columns->Count == 0)
        {
            teamPlayersGrid->Columns->Add(L"ID", L"ID");
            teamPlayersGrid->Columns->Add(L"Nickname", L"Никнейм");
            teamPlayersGrid->Columns->Add(L"Full Name", L"Полное имя");
            teamPlayersGrid->Columns->Add(L"Role", L"Роль");
            teamPlayersGrid->Columns->Add(L"Nationality", L"Национальность");
            teamPlayersGrid->Columns->Add(L"Rating", L"Рейтинг");
        }
        std::vector<Player> players = playerRepo->getPlayersByTeam(selectedTeamId);
        for (int i = 0; i < (int)players.size(); i++)
        {
            Player p = players[i];
            teamPlayersGrid->Rows->Add(p.getId(), gcnew String(p.getNickname().c_str()), gcnew String(p.getFullName().c_str()), gcnew String(p.getRole().c_str()), gcnew String(p.getNationality().c_str()), p.getRating());
        }

        teamMatchesHistoryGrid->Rows->Clear();
        if (teamMatchesHistoryGrid->Columns->Count == 0)
        {
            teamMatchesHistoryGrid->Columns->Add(L"ID", L"ID");
            teamMatchesHistoryGrid->Columns->Add(L"Tournament", L"Турнир");
            teamMatchesHistoryGrid->Columns->Add(L"Opponent", L"Соперник");
            teamMatchesHistoryGrid->Columns->Add(L"Score", L"Счет");
            teamMatchesHistoryGrid->Columns->Add(L"Date", L"Дата");
            teamMatchesHistoryGrid->Columns->Add(L"Result", L"Результат");
        }
        std::vector<Match> allMatches = matchRepo->getAllMatches();
        for (int i = 0; i < (int)allMatches.size(); i++)
        {
            Match m = allMatches[i];
            if ((m.getTeam1Id() == selectedTeamId || m.getTeam2Id() == selectedTeamId) && m.getStatus() == "completed")
            {
                int opponentId = (m.getTeam1Id() == selectedTeamId) ? m.getTeam2Id() : m.getTeam1Id();
                int teamScore = (m.getTeam1Id() == selectedTeamId) ? m.getTeam1Score() : m.getTeam2Score();
                int opponentScore = (m.getTeam1Id() == selectedTeamId) ? m.getTeam2Score() : m.getTeam1Score();
                String^ result;
                if (teamScore > opponentScore) result = L"Победа \u2713";
                else if (teamScore < opponentScore) result = L"Поражение \u2717";
                else result = L"Ничья =";
                teamMatchesHistoryGrid->Rows->Add(m.getId(), gcnew String(tournamentRepo->getTournamentDisplayName(m.getTournamentId()).c_str()), gcnew String(teamRepo->getTeamDisplayName(opponentId).c_str()), String::Format(L"{0} - {1}", teamScore, opponentScore), gcnew String(m.getMatchDate().c_str()), result);
            }
        }

        teamUpcomingMatchesGrid->Rows->Clear();
        if (teamUpcomingMatchesGrid->Columns->Count == 0)
        {
            teamUpcomingMatchesGrid->Columns->Add(L"ID", L"ID");
            teamUpcomingMatchesGrid->Columns->Add(L"Tournament", L"Турнир");
            teamUpcomingMatchesGrid->Columns->Add(L"Opponent", L"Соперник");
            teamUpcomingMatchesGrid->Columns->Add(L"Date", L"Дата");
            teamUpcomingMatchesGrid->Columns->Add(L"Stage", L"Стадия");
        }
        for (int i = 0; i < (int)allMatches.size(); i++)
        {
            Match m = allMatches[i];
            if ((m.getTeam1Id() == selectedTeamId || m.getTeam2Id() == selectedTeamId) && m.getStatus() == "scheduled")
            {
                int opponentId = (m.getTeam1Id() == selectedTeamId) ? m.getTeam2Id() : m.getTeam1Id();
                teamUpcomingMatchesGrid->Rows->Add(m.getId(), gcnew String(tournamentRepo->getTournamentDisplayName(m.getTournamentId()).c_str()), gcnew String(teamRepo->getTeamDisplayName(opponentId).c_str()), gcnew String(m.getMatchDate().c_str()), gcnew String(m.getStage().c_str()));
            }
        }
    }

    void HandleBackToTeams(Object^ sender, EventArgs^ e)
    {
        teamDetailPanel->Visible = false;
        SwitchPage(3);
    }

    // ========== ОБРАБОТЧИКИ ВИДОВ СПОРТА ==========
    void HandleSportCellClick(Object^ sender, DataGridViewCellEventArgs^ e)
    {
        if (e->RowIndex >= 0)
        {
            txtSportId->Text = sportsGrid->Rows[e->RowIndex]->Cells[0]->Value->ToString();
            txtSportName->Text = sportsGrid->Rows[e->RowIndex]->Cells[1]->Value->ToString();
            txtSportIcon->Text = sportsGrid->Rows[e->RowIndex]->Cells[2]->Value->ToString();
            String^ color = sportsGrid->Rows[e->RowIndex]->Cells[3]->Value->ToString();
            for (int i = 0; i < cmbSportColor->Items->Count; i++)
            {
                if (cmbSportColor->Items[i]->ToString()->Contains(color))
                {
                    cmbSportColor->SelectedIndex = i; break;
                }
            }
            int id = Int32::Parse(txtSportId->Text);
            Sport sport = sportRepo->getSportById(id);
            txtSportDescription->Text = gcnew String(sport.getDescription().c_str());
        }
    }

    void HandleAddSport(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrWhiteSpace(txtSportName->Text))
        {
            MessageBox::Show(L"Введите название дисциплины!", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        std::string name = msclr::interop::marshal_as<std::string>(txtSportName->Text);
        std::string icon = msclr::interop::marshal_as<std::string>(txtSportIcon->Text);
        String^ selectedColor = cmbSportColor->SelectedItem->ToString();
        std::string color = msclr::interop::marshal_as<std::string>(selectedColor->Substring(0, 7));
        std::string description = msclr::interop::marshal_as<std::string>(txtSportDescription->Text);
        if (sportRepo->addSport(Sport(0, name, icon, color, description)))
        {
            LoadSports();
            LoadAllFilters();
            UpdateDashboardStats();
            ClearSportForm();
            MessageBox::Show(L"Дисциплина успешно добавлена!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleUpdateSport(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtSportId->Text))
        {
            MessageBox::Show(L"Выберите дисциплину для редактирования!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtSportId->Text);
        std::string name = msclr::interop::marshal_as<std::string>(txtSportName->Text);
        std::string icon = msclr::interop::marshal_as<std::string>(txtSportIcon->Text);
        String^ selectedColor = cmbSportColor->SelectedItem->ToString();
        std::string color = msclr::interop::marshal_as<std::string>(selectedColor->Substring(0, 7));
        std::string description = msclr::interop::marshal_as<std::string>(txtSportDescription->Text);
        if (sportRepo->updateSport(Sport(id, name, icon, color, description)))
        {
            LoadSports();
            LoadAllFilters();
            MessageBox::Show(L"Дисциплина успешно обновлена!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleDeleteSport(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtSportId->Text))
        {
            MessageBox::Show(L"Выберите дисциплину для удаления!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        if (MessageBox::Show(L"Вы уверены? Все связанные турниры и команды будут удалены!", L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes)
        {
            int id = Int32::Parse(txtSportId->Text);
            if (sportRepo->deleteSport(id))
            {
                LoadSports();
                LoadTournaments();
                LoadTeams();
                LoadFilteredMatches();
                LoadAllFilters();
                UpdateDashboardStats();
                ClearSportForm();
                MessageBox::Show(L"Дисциплина успешно удалена!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
    }

    void ClearSportForm()
    {
        txtSportId->Text = "";
        txtSportName->Text = "";
        txtSportIcon->Text = "";
        cmbSportColor->SelectedIndex = 0;
        txtSportDescription->Text = "";
    }

    // ========== ОБРАБОТЧИКИ ТУРНИРОВ ==========
    void HandleTournamentCellClick(Object^ sender, DataGridViewCellEventArgs^ e)
    {
        if (e->RowIndex >= 0)
        {
            txtTournamentId->Text = tournamentsGrid->Rows[e->RowIndex]->Cells[0]->Value->ToString();
            String^ sportName = tournamentsGrid->Rows[e->RowIndex]->Cells[1]->Value->ToString();
            CyberComboBox^ cmbTournamentSportForm = FindCmbTournamentSportForm();
            if (cmbTournamentSportForm != nullptr)
            {
                for (int i = 0; i < cmbTournamentSportForm->Items->Count; i++)
                {
                    if (cmbTournamentSportForm->Items[i]->ToString()->Contains(sportName))
                    {
                        cmbTournamentSportForm->SelectedIndex = i;
                        break;
                    }
                }
            }
            txtTournamentName->Text = tournamentsGrid->Rows[e->RowIndex]->Cells[2]->Value->ToString();
            int id = Int32::Parse(txtTournamentId->Text);
            Tournament t = tournamentRepo->getTournamentById(id);
            txtTournamentPrize->Text = t.getPrizePool().ToString();
            String^ status = tournamentsGrid->Rows[e->RowIndex]->Cells[6]->Value->ToString();
            for (int i = 0; i < cmbTournamentStatus->Items->Count; i++)
            {
                if (cmbTournamentStatus->Items[i]->ToString() == status)
                {
                    cmbTournamentStatus->SelectedIndex = i;
                    break;
                }
            }
        }
    }

    void HandleAddTournament(Object^ sender, EventArgs^ e)
    {
        CyberComboBox^ cmbTournamentSportForm = FindCmbTournamentSportForm();
        if (cmbTournamentSportForm == nullptr || cmbTournamentSportForm->SelectedIndex < 0 || String::IsNullOrWhiteSpace(txtTournamentName->Text))
        {
            MessageBox::Show(L"Выберите дисциплину и введите название!", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        String^ sportStr = cmbTournamentSportForm->SelectedItem->ToString();
        int sportId = Int32::Parse(sportStr->Substring(0, sportStr->IndexOf(L" ")));
        std::string name = msclr::interop::marshal_as<std::string>(txtTournamentName->Text);
        std::string startDate = msclr::interop::marshal_as<std::string>(dtpTournamentStart->Value.ToString("yyyy-MM-dd"));
        std::string endDate = msclr::interop::marshal_as<std::string>(dtpTournamentEnd->Value.ToString("yyyy-MM-dd"));
        String^ prizeText = txtTournamentPrize->Text->Replace(L",", L".");
        double prizePool = Double::Parse(prizeText, System::Globalization::CultureInfo::InvariantCulture);
        std::string status = msclr::interop::marshal_as<std::string>(cmbTournamentStatus->SelectedItem->ToString());
        if (tournamentRepo->addTournament(Tournament(0, sportId, name, startDate, endDate, prizePool, status, "")))
        {
            LoadFilteredTournaments();
            LoadMatchFilters();
            LoadComboBoxes();
            UpdateDashboardStats();
            ClearTournamentForm();
            MessageBox::Show(L"Турнир успешно добавлен!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleUpdateTournament(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTournamentId->Text))
        {
            MessageBox::Show(L"Выберите турнир для редактирования!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        CyberComboBox^ cmbTournamentSportForm = FindCmbTournamentSportForm();
        if (cmbTournamentSportForm == nullptr || cmbTournamentSportForm->SelectedIndex < 0)
        {
            MessageBox::Show(L"Выберите дисциплину!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtTournamentId->Text);
        String^ sportStr = cmbTournamentSportForm->SelectedItem->ToString();
        int sportId = Int32::Parse(sportStr->Substring(0, sportStr->IndexOf(L" ")));
        std::string name = msclr::interop::marshal_as<std::string>(txtTournamentName->Text);
        std::string startDate = msclr::interop::marshal_as<std::string>(dtpTournamentStart->Value.ToString("yyyy-MM-dd"));
        std::string endDate = msclr::interop::marshal_as<std::string>(dtpTournamentEnd->Value.ToString("yyyy-MM-dd"));
        String^ prizeText = txtTournamentPrize->Text->Replace(L",", L".");
        double prizePool = Double::Parse(prizeText, System::Globalization::CultureInfo::InvariantCulture);
        std::string status = msclr::interop::marshal_as<std::string>(cmbTournamentStatus->SelectedItem->ToString());
        if (tournamentRepo->updateTournament(Tournament(id, sportId, name, startDate, endDate, prizePool, status, "")))
        {
            LoadFilteredTournaments();
            LoadMatchFilters();
            LoadComboBoxes();
            UpdateDashboardStats();
            MessageBox::Show(L"Турнир успешно обновлен!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleDeleteTournament(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTournamentId->Text))
        {
            MessageBox::Show(L"Выберите турнир для удаления!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        if (MessageBox::Show(L"Вы уверены? Все матчи будут удалены!", L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes)
        {
            int id = Int32::Parse(txtTournamentId->Text);
            if (tournamentRepo->deleteTournament(id))
            {
                LoadFilteredTournaments();
                LoadFilteredMatches();
                LoadMatchFilters();
                LoadComboBoxes();
                UpdateDashboardStats();
                ClearTournamentForm();
                MessageBox::Show(L"Турнир успешно удален!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
    }

    void HandleStartTournament(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTournamentId->Text))
        {
            MessageBox::Show(L"Выберите турнир!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtTournamentId->Text);
        if (tournamentRepo->updateTournamentStatus(id, "active"))
        {
            LoadFilteredTournaments();
            UpdateDashboardStats();
            MessageBox::Show(L"Турнир начат! \U0001f525", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleManageTournamentTeams(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTournamentId->Text))
        {
            MessageBox::Show(L"Выберите турнир для управления командами!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int tournamentId = Int32::Parse(txtTournamentId->Text);
        TournamentTeamsForm^ teamsForm = gcnew TournamentTeamsForm(db, tournamentId);
        teamsForm->ShowDialog();
        delete teamsForm;
        LoadFilteredTournaments();
        UpdateDashboardStats();
    }

    void ClearTournamentForm()
    {
        txtTournamentId->Text = "";
        txtTournamentName->Text = "";
        dtpTournamentStart->Value = DateTime::Now;
        dtpTournamentEnd->Value = DateTime::Now.AddMonths(1);
        txtTournamentPrize->Text = L"0";
        cmbTournamentStatus->SelectedIndex = 0;
    }

    // ========== ОБРАБОТЧИКИ КОМАНД ==========
    void HandleTeamCellClick(Object^ sender, DataGridViewCellEventArgs^ e)
    {
        if (e->RowIndex >= 0)
        {
            txtTeamId->Text = teamsGrid->Rows[e->RowIndex]->Cells[0]->Value->ToString();
            txtTeamName->Text = teamsGrid->Rows[e->RowIndex]->Cells[2]->Value->ToString();
            txtTeamTag->Text = teamsGrid->Rows[e->RowIndex]->Cells[3]->Value->ToString();
            txtTeamLogo->Text = teamsGrid->Rows[e->RowIndex]->Cells[4]->Value->ToString();
            String^ sportName = teamsGrid->Rows[e->RowIndex]->Cells[1]->Value->ToString();
            CyberComboBox^ cmbTeamSportForm = FindCmbTeamSportForm();
            if (cmbTeamSportForm != nullptr)
            {
                for (int i = 0; i < cmbTeamSportForm->Items->Count; i++)
                {
                    if (cmbTeamSportForm->Items[i]->ToString()->Contains(sportName))
                    {
                        cmbTeamSportForm->SelectedIndex = i; break;
                    }
                }
            }
        }
    }

    void HandleAddTeam(Object^ sender, EventArgs^ e)
    {
        CyberComboBox^ cmbTeamSportForm = FindCmbTeamSportForm();
        if (cmbTeamSportForm == nullptr || cmbTeamSportForm->SelectedIndex < 0 || String::IsNullOrWhiteSpace(txtTeamName->Text))
        {
            MessageBox::Show(L"Заполните обязательные поля!", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        String^ sportStr = cmbTeamSportForm->SelectedItem->ToString();
        int sportId = Int32::Parse(sportStr->Substring(0, sportStr->IndexOf(L" ")));
        std::string name = msclr::interop::marshal_as<std::string>(txtTeamName->Text);
        std::string tag = msclr::interop::marshal_as<std::string>(txtTeamTag->Text);
        std::string logo = msclr::interop::marshal_as<std::string>(txtTeamLogo->Text);
        if (teamRepo->addTeam(Team(0, sportId, name, tag, logo, 0, 0, 0, "")))
        {
            LoadFilteredTeams();
            LoadComboBoxes();
            UpdateDashboardStats();
            ClearTeamForm();
            MessageBox::Show(L"Команда успешно добавлена!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleUpdateTeam(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTeamId->Text))
        {
            MessageBox::Show(L"Выберите команду для редактирования!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        CyberComboBox^ cmbTeamSportForm = FindCmbTeamSportForm();
        if (cmbTeamSportForm == nullptr || cmbTeamSportForm->SelectedIndex < 0)
        {
            MessageBox::Show(L"Выберите дисциплину!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtTeamId->Text);
        String^ sportStr = cmbTeamSportForm->SelectedItem->ToString();
        int sportId = Int32::Parse(sportStr->Substring(0, sportStr->IndexOf(L" ")));
        std::string name = msclr::interop::marshal_as<std::string>(txtTeamName->Text);
        std::string tag = msclr::interop::marshal_as<std::string>(txtTeamTag->Text);
        std::string logo = msclr::interop::marshal_as<std::string>(txtTeamLogo->Text);
        Team team = teamRepo->getTeamById(id);
        if (teamRepo->updateTeam(Team(id, sportId, name, tag, logo, team.getWins(), team.getLosses(), team.getPoints(), "")))
        {
            LoadFilteredTeams();
            LoadComboBoxes();
            UpdateDashboardStats();
            MessageBox::Show(L"Команда успешно обновлена!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleDeleteTeam(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtTeamId->Text))
        {
            MessageBox::Show(L"Выберите команду для удаления!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        if (MessageBox::Show(L"Вы уверены? Все игроки станут свободными агентами!", L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes)
        {
            int id = Int32::Parse(txtTeamId->Text);
            if (teamRepo->deleteTeam(id))
            {
                LoadFilteredTeams();
                LoadPlayers();
                LoadComboBoxes();
                UpdateDashboardStats();
                ClearTeamForm();
                MessageBox::Show(L"Команда успешно удалена!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
    }

    void ClearTeamForm()
    {
        txtTeamId->Text = "";
        txtTeamName->Text = "";
        txtTeamTag->Text = "";
        txtTeamLogo->Text = L"\U0001f3e2";
    }

    // ========== ОБРАБОТЧИКИ ИГРОКОВ ==========
    void HandlePlayerCellClick(Object^ sender, DataGridViewCellEventArgs^ e)
    {
        if (e->RowIndex >= 0)
        {
            txtPlayerId->Text = playersGrid->Rows[e->RowIndex]->Cells[0]->Value->ToString();
            String^ teamName = playersGrid->Rows[e->RowIndex]->Cells[1]->Value->ToString();
            for (int i = 0; i < cmbPlayerTeam->Items->Count; i++)
            {
                if (cmbPlayerTeam->Items[i]->ToString()->Contains(teamName) || (teamName == L"Free Agent" && i == 0))
                {
                    cmbPlayerTeam->SelectedIndex = i; break;
                }
            }
            txtPlayerNickname->Text = playersGrid->Rows[e->RowIndex]->Cells[2]->Value->ToString();
            txtPlayerFullName->Text = playersGrid->Rows[e->RowIndex]->Cells[3]->Value != nullptr ? playersGrid->Rows[e->RowIndex]->Cells[3]->Value->ToString() : "";
            txtPlayerRole->Text = playersGrid->Rows[e->RowIndex]->Cells[4]->Value != nullptr ? playersGrid->Rows[e->RowIndex]->Cells[4]->Value->ToString() : "";
            txtPlayerNationality->Text = playersGrid->Rows[e->RowIndex]->Cells[5]->Value != nullptr ? playersGrid->Rows[e->RowIndex]->Cells[5]->Value->ToString() : "";
            txtPlayerRating->Text = playersGrid->Rows[e->RowIndex]->Cells[6]->Value->ToString();
        }
    }

    void HandleAddPlayer(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrWhiteSpace(txtPlayerNickname->Text))
        {
            MessageBox::Show(L"Введите никнейм игрока!", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        String^ teamStr = cmbPlayerTeam->SelectedItem->ToString();
        int teamId = Int32::Parse(teamStr->Substring(0, teamStr->IndexOf(L" ")));
        std::string nickname = msclr::interop::marshal_as<std::string>(txtPlayerNickname->Text);
        std::string fullName = msclr::interop::marshal_as<std::string>(txtPlayerFullName->Text);
        std::string role = msclr::interop::marshal_as<std::string>(txtPlayerRole->Text);
        std::string nationality = msclr::interop::marshal_as<std::string>(txtPlayerNationality->Text);
        int rating = Int32::Parse(txtPlayerRating->Text);
        if (playerRepo->addPlayer(Player(0, teamId, nickname, fullName, role, nationality, rating, "")))
        {
            LoadPlayers();
            UpdateDashboardStats();
            ClearPlayerForm();
            MessageBox::Show(L"Игрок успешно добавлен!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleUpdatePlayer(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtPlayerId->Text))
        {
            MessageBox::Show(L"Выберите игрока для редактирования!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtPlayerId->Text);
        String^ teamStr = cmbPlayerTeam->SelectedItem->ToString();
        int teamId = Int32::Parse(teamStr->Substring(0, teamStr->IndexOf(L" ")));
        std::string nickname = msclr::interop::marshal_as<std::string>(txtPlayerNickname->Text);
        std::string fullName = msclr::interop::marshal_as<std::string>(txtPlayerFullName->Text);
        std::string role = msclr::interop::marshal_as<std::string>(txtPlayerRole->Text);
        std::string nationality = msclr::interop::marshal_as<std::string>(txtPlayerNationality->Text);
        int rating = Int32::Parse(txtPlayerRating->Text);
        if (playerRepo->updatePlayer(Player(id, teamId, nickname, fullName, role, nationality, rating, "")))
        {
            LoadPlayers();
            UpdateDashboardStats();
            MessageBox::Show(L"Игрок успешно обновлен!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleDeletePlayer(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtPlayerId->Text))
        {
            MessageBox::Show(L"Выберите игрока для удаления!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        if (MessageBox::Show(L"Вы уверены?", L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes)
        {
            int id = Int32::Parse(txtPlayerId->Text);
            if (playerRepo->deletePlayer(id))
            {
                LoadPlayers();
                UpdateDashboardStats();
                ClearPlayerForm();
                MessageBox::Show(L"Игрок удален!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
    }

    void ClearPlayerForm()
    {
        txtPlayerId->Text = "";
        cmbPlayerTeam->SelectedIndex = 0;
        txtPlayerNickname->Text = "";
        txtPlayerFullName->Text = "";
        txtPlayerRole->Text = "";
        txtPlayerNationality->Text = "";
        txtPlayerRating->Text = L"1000";
    }

    // ========== ОБРАБОТЧИКИ МАТЧЕЙ ==========
    void HandleMatchCellClick(Object^ sender, DataGridViewCellEventArgs^ e)
    {
        if (e->RowIndex >= 0)
        {
            txtMatchId->Text = matchesGrid->Rows[e->RowIndex]->Cells[0]->Value->ToString();
            String^ tournamentName = matchesGrid->Rows[e->RowIndex]->Cells[1]->Value->ToString();
            for (int i = 0; i < cmbMatchTournament->Items->Count; i++)
            {
                if (cmbMatchTournament->Items[i]->ToString()->Contains(tournamentName))
                {
                    cmbMatchTournament->SelectedIndex = i; break;
                }
            }
            String^ team1Name = matchesGrid->Rows[e->RowIndex]->Cells[2]->Value->ToString();
            for (int i = 0; i < cmbMatchTeam1->Items->Count; i++)
            {
                if (cmbMatchTeam1->Items[i]->ToString()->Contains(team1Name))
                {
                    cmbMatchTeam1->SelectedIndex = i; break;
                }
            }
            String^ team2Name = matchesGrid->Rows[e->RowIndex]->Cells[3]->Value->ToString();
            for (int i = 0; i < cmbMatchTeam2->Items->Count; i++)
            {
                if (cmbMatchTeam2->Items[i]->ToString()->Contains(team2Name))
                {
                    cmbMatchTeam2->SelectedIndex = i; break;
                }
            }
            int id = Int32::Parse(txtMatchId->Text);
            Match m = matchRepo->getMatchById(id);
            txtMatchScore1->Text = m.getTeam1Score().ToString();
            txtMatchScore2->Text = m.getTeam2Score().ToString();
            String^ stage = matchesGrid->Rows[e->RowIndex]->Cells[6]->Value->ToString();
            for (int i = 0; i < cmbMatchStage->Items->Count; i++)
            {
                if (cmbMatchStage->Items[i]->ToString() == stage)
                {
                    cmbMatchStage->SelectedIndex = i; break;
                }
            }
            String^ status = matchesGrid->Rows[e->RowIndex]->Cells[7]->Value->ToString();
            for (int i = 0; i < cmbMatchStatus->Items->Count; i++)
            {
                if (cmbMatchStatus->Items[i]->ToString() == status)
                {
                    cmbMatchStatus->SelectedIndex = i; break;
                }
            }
        }
    }

    void HandleAddMatch(Object^ sender, EventArgs^ e)
    {
        if (cmbMatchTournament->SelectedIndex < 0 || cmbMatchTeam1->SelectedIndex < 0 || cmbMatchTeam2->SelectedIndex < 0)
        {
            MessageBox::Show(L"Заполните обязательные поля!", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        String^ tournamentStr = cmbMatchTournament->SelectedItem->ToString();
        int tournamentId = Int32::Parse(tournamentStr->Substring(0, tournamentStr->IndexOf(L" ")));
        String^ team1Str = cmbMatchTeam1->SelectedItem->ToString();
        int team1Id = Int32::Parse(team1Str->Substring(0, team1Str->IndexOf(L" ")));
        String^ team2Str = cmbMatchTeam2->SelectedItem->ToString();
        int team2Id = Int32::Parse(team2Str->Substring(0, team2Str->IndexOf(L" ")));
        if (team1Id == team2Id)
        {
            MessageBox::Show(L"Команды должны быть разными!", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        std::string matchDate = msclr::interop::marshal_as<std::string>(dtpMatchDate->Value.ToString("yyyy-MM-dd HH:mm:ss"));
        int score1 = Int32::Parse(txtMatchScore1->Text);
        int score2 = Int32::Parse(txtMatchScore2->Text);
        std::string stage = msclr::interop::marshal_as<std::string>(cmbMatchStage->SelectedItem->ToString());
        std::string status = msclr::interop::marshal_as<std::string>(cmbMatchStatus->SelectedItem->ToString());
        if (matchRepo->addMatch(Match(0, tournamentId, team1Id, team2Id, matchDate, score1, score2, stage, status, "")))
        {
            LoadFilteredMatches();
            UpdateDashboardStats();
            ClearMatchForm();
            MessageBox::Show(L"Матч успешно добавлен!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleUpdateMatch(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtMatchId->Text))
        {
            MessageBox::Show(L"Выберите матч для редактирования!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtMatchId->Text);
        String^ tournamentStr = cmbMatchTournament->SelectedItem->ToString();
        int tournamentId = Int32::Parse(tournamentStr->Substring(0, tournamentStr->IndexOf(L" ")));
        String^ team1Str = cmbMatchTeam1->SelectedItem->ToString();
        int team1Id = Int32::Parse(team1Str->Substring(0, team1Str->IndexOf(L" ")));
        String^ team2Str = cmbMatchTeam2->SelectedItem->ToString();
        int team2Id = Int32::Parse(team2Str->Substring(0, team2Str->IndexOf(L" ")));
        std::string matchDate = msclr::interop::marshal_as<std::string>(dtpMatchDate->Value.ToString("yyyy-MM-dd HH:mm:ss"));
        int score1 = Int32::Parse(txtMatchScore1->Text);
        int score2 = Int32::Parse(txtMatchScore2->Text);
        std::string stage = msclr::interop::marshal_as<std::string>(cmbMatchStage->SelectedItem->ToString());
        std::string status = msclr::interop::marshal_as<std::string>(cmbMatchStatus->SelectedItem->ToString());
        if (matchRepo->updateMatch(Match(id, tournamentId, team1Id, team2Id, matchDate, score1, score2, stage, status, "")))
        {
            LoadFilteredMatches();
            UpdateDashboardStats();
            MessageBox::Show(L"Матч успешно обновлен!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleDeleteMatch(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtMatchId->Text))
        {
            MessageBox::Show(L"Выберите матч для удаления!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        if (MessageBox::Show(L"Вы уверены?", L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes)
        {
            int id = Int32::Parse(txtMatchId->Text);
            if (matchRepo->deleteMatch(id))
            {
                LoadFilteredMatches();
                UpdateDashboardStats();
                ClearMatchForm();
                MessageBox::Show(L"Матч удален!", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }
    }

    void HandleStartMatch(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtMatchId->Text))
        {
            MessageBox::Show(L"Выберите матч!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtMatchId->Text);
        if (matchRepo->updateMatchStatus(id, "live"))
        {
            LoadFilteredMatches();
            UpdateDashboardStats();
            MessageBox::Show(L"Матч начат! \U0001f525", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void HandleFinishMatch(Object^ sender, EventArgs^ e)
    {
        if (String::IsNullOrEmpty(txtMatchId->Text))
        {
            MessageBox::Show(L"Выберите матч!", L"Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }
        int id = Int32::Parse(txtMatchId->Text);
        int score1 = Int32::Parse(txtMatchScore1->Text);
        int score2 = Int32::Parse(txtMatchScore2->Text);
        matchRepo->updateMatchScore(id, score1, score2);
        if (matchRepo->updateMatchStatus(id, "completed"))
        {
            Match m = matchRepo->getMatchById(id);
            Team team1 = teamRepo->getTeamById(m.getTeam1Id());
            Team team2 = teamRepo->getTeamById(m.getTeam2Id());
            if (score1 > score2)
            {
                teamRepo->updateTeamStats(team1.getId(), team1.getWins() + 1, team1.getLosses(), team1.getPoints() + 3);
                teamRepo->updateTeamStats(team2.getId(), team2.getWins(), team2.getLosses() + 1, team2.getPoints());
            }
            else if (score2 > score1)
            {
                teamRepo->updateTeamStats(team1.getId(), team1.getWins(), team1.getLosses() + 1, team1.getPoints());
                teamRepo->updateTeamStats(team2.getId(), team2.getWins() + 1, team2.getLosses(), team2.getPoints() + 3);
            }
            LoadFilteredMatches();
            LoadFilteredTeams();
            UpdateDashboardStats();
            MessageBox::Show(L"Матч завершен! \U0001f3c1", L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    }

    void ClearMatchForm()
    {
        txtMatchId->Text = "";
        cmbMatchTournament->SelectedIndex = -1;
        cmbMatchTeam1->SelectedIndex = -1;
        cmbMatchTeam2->SelectedIndex = -1;
        dtpMatchDate->Value = DateTime::Now.AddDays(1);
        txtMatchScore1->Text = L"0";
        txtMatchScore2->Text = L"0";
        cmbMatchStage->SelectedIndex = 0;
        cmbMatchStatus->SelectedIndex = 0;
    }
};