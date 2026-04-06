/**
 * The Adversary
 * Game UI mode
 *
 * @details This module runs the game in UI mode.
 * @author The dark net programmers
 */

#include <cstdint>
#include <iostream>

#include "raylib.h"

#include "ui.h"

#include "../game/game.h"

// UI configuration

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define TITLE_HEIGHT 280

#define BUTTON_WIDTH 320
#define BUTTON_HEIGHT 64
#define BUTTON_GAP 16

#define TITLE_SIZE 128
#define TEXT_SIZE 40

#define BOARD_X 0.0f
#define BOARD_Y 0.0f
#define CELL_SIZE (WINDOW_HEIGHT / BOARD_SIZE)

#define PANEL_LEFT (WINDOW_HEIGHT)
#define PANEL_WIDTH (WINDOW_WIDTH - WINDOW_HEIGHT)
#define PANEL_PADDING 24

#define STATUS_CARD_TOP 28
#define STATUS_CARD_HEIGHT 96

#define SCORE_CARD_TOP ((WINDOW_HEIGHT - SCORE_CARD_HEIGHT) / 2)
#define SCORE_CARD_HEIGHT 256
#define SCORE_CARD_DISK_X (PANEL_LEFT + PANEL_WIDTH / 3 - SCORE_CARD_SCORE_WIDTH / 2)
#define SCORE_CARD_DISK_RADIUS 24
#define SCORE_CARD_SCORE_X (SCORE_CARD_DISK_X + SCORE_CARD_DISK_RADIUS)
#define SCORE_CARD_SCORE_WIDTH 64
#define SCORE_CARD_TIME_X (PANEL_LEFT + 2 * PANEL_WIDTH / 3 - SCORE_CARD_TIME_WIDTH / 2)
#define SCORE_CARD_TIME_WIDTH 128
#define SCORE_CARD_NAME_X (PANEL_LEFT + PANEL_WIDTH / 2 - SCORE_CARD_NAME_WIDTH / 2)
#define SCORE_CARD_NAME_WIDTH 128
#define SCORE_CARD_ROW1_Y (WINDOW_HEIGHT / 2 - 2.3f * TEXT_SIZE)
#define SCORE_CARD_ROW2_Y (WINDOW_HEIGHT / 2 + 0.7f * TEXT_SIZE)

#define CONTROL_BUTTON_TOP (WINDOW_HEIGHT - PANEL_PADDING - BUTTON_HEIGHT)

#define STRING_APP "The Adversary"

#define STRING_PLAY_GAME "Start game"

#define STRING_BLACK "Black"
#define STRING_WHITE "White"

#define STRING_HUMAN "Human"

#define STRING_BLACK_TO_MOVE "Black to move"
#define STRING_WHITE_TO_MOVE "White to move"
#define STRING_BLACK_WINS "Black wins!"
#define STRING_WHITE_WINS "White wins!"
#define STRING_DRAW "Draw"

#define STRING_PLAY_AGAIN "Play again?"
#define STRING_LEAVE_GAME "Leave game"

static const Color BACKGROUND_COLOR = {12, 16, 24, 255};

static const Color ACTIVE_TEXT_COLOR = {210, 214, 224, 255};
static const Color SELECTED_TEXT_COLOR = {16, 18, 24, 255};
static const Color MUTED_TEXT_COLOR = {122, 130, 148, 255};

static const Color BUTTON_COLOR = {34, 40, 52, 255};
static const Color BUTTON_HOVER_COLOR = {47, 55, 70, 255};
static const Color BUTTON_SELECTED_COLOR = {92, 140, 255, 255};
static const Color BUTTON_BORDER_COLOR = {66, 74, 92, 255};

static const Color BOARD_FRAME_COLOR = {18, 60, 42, 255};
static const Color BOARD_CELL_COLOR = {22, 92, 62, 255};
static const Color BOARD_LINE_COLOR = {12, 34, 26, 255};
static const Color LAST_MOVE_COLOR = {255, 204, 92, 255};

static const Color BLACK_SQUARE_COLOR = {22, 24, 28, 255};
static const Color BLACK_SQUARE_OUTLINE_COLOR = {8, 10, 14, 255};
static const Color WHITE_SQUARE_COLOR = {236, 239, 246, 255};
static const Color WHITE_SQUARE_OUTLINE_COLOR = {168, 176, 194, 255};

static const char *PHASE_LABELS[] = {
    STRING_BLACK_TO_MOVE,
    STRING_WHITE_TO_MOVE,
    STRING_BLACK_WINS,
    STRING_WHITE_WINS,
    STRING_DRAW,
};

// UI state

static bool in_game = false;

enum PlayerType
{
    PLAYER_HUMAN = 0,
    PLAYER_AI = 1,
};

static PlayerType black_player_type = PLAYER_HUMAN;
static PlayerType white_player_type = PLAYER_AI;

static const AgentInstance *agent_instances;

static Game game;

static bool has_last_move = false;
static GamePosition last_move = {-1, -1};

static double black_elapsed_seconds = 0.0;
static double white_elapsed_seconds = 0.0;
static double last_timer_update_time = 0.0;
static double last_stop_requested_ui_update_time = 0.0;

static bool close_requested = false;
static bool stop_requested = false;
static bool agent_running = false;

void LoadUI(const AgentInstance *instances)
{
    agent_instances = instances;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, STRING_APP);
    last_timer_update_time = GetTime();

    SetTargetFPS(60);

    SetExitKey(KEY_NULL);
}

void UnloadUI()
{
    CloseWindow();
}

/**
 * @brief Draws the background of the UI.
 */
static void DrawBackground()
{
    ClearBackground(BACKGROUND_COLOR);
}

/**
 * @brief Draws centered text at the specified position.
 */
static void DrawCenteredText(const char *text, Rectangle bounds, int font_size, Color color)
{
    int32_t text_width = MeasureText(text, font_size);
    DrawText(text,
             (int32_t)(bounds.x + bounds.width / 2 - text_width / 2),
             (int32_t)(bounds.y + bounds.height / 2 - font_size / 2),
             font_size,
             color);
};

/**
 * @brief Updates a button, checking for hover and click events and drawing it.
 */
static bool UpdateButton(const char *text,
                         Vector2 position,
                         int32_t font_size,
                         bool selected = false)
{
    Rectangle bounds = {position.x, position.y, BUTTON_WIDTH, BUTTON_HEIGHT};
    float roundness = 0.2f;

    bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
    bool clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                   CheckCollisionPointRec(GetMousePosition(), bounds);

    Color fill_color = BUTTON_COLOR;
    Color text_color = ACTIVE_TEXT_COLOR;

    if (selected)
    {
        fill_color = BUTTON_SELECTED_COLOR;
        text_color = SELECTED_TEXT_COLOR;
    }
    else if (hovered)
        fill_color = BUTTON_HOVER_COLOR;

    if (hovered)
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

    DrawRectangleRounded(bounds, roundness, 8, fill_color);
    DrawRectangleRoundedLinesEx(bounds, roundness, 8, 2.0f, BUTTON_BORDER_COLOR);

    DrawCenteredText(text, bounds, font_size, text_color);

    return clicked;
}

/**
 * @brief Gets the name of the agent at the given index in the agent instances array. This is used to display the agent names in the UI when selecting player types.
 *
 * @param agent_index The index of the agent in the agent instances array.
 *
 * @return The name of the agent at the given index, or "Unknown agent" if the index is out of bounds.
 */
static const char *GetAgentName(int agent_index)
{
    if (agent_index < 0 || agent_index >= 2)
        return "Unknown agent";

    return agent_instances[agent_index].descriptor->name;
}

/**
 * @brief Updates a player selector, which consists of two buttons for selecting between human and AI players.
 */
static void UpdatePlayerSelector(const char *side_label, int agent_index, Vector2 position, PlayerType &selected_type)
{
    Rectangle bounds = {position.x, position.y, BUTTON_WIDTH, BUTTON_HEIGHT};
    DrawCenteredText(side_label, bounds, TEXT_SIZE, MUTED_TEXT_COLOR);

    position.y += bounds.height + BUTTON_GAP;
    if (UpdateButton(STRING_HUMAN, position, TEXT_SIZE, selected_type == PLAYER_HUMAN))
        selected_type = PLAYER_HUMAN;

    position.y += bounds.height + BUTTON_GAP;
    if (UpdateButton(GetAgentName(agent_index), position, TEXT_SIZE, selected_type == PLAYER_AI))
        selected_type = PLAYER_AI;
}

/**
 * @brief Updates the start menu UI, allowing the user to select player types and start the game.
 */
static void UpdateStartMenuUI()
{
    float y = 0;

    Rectangle bounds = {0, y, WINDOW_WIDTH, TITLE_HEIGHT};
    DrawCenteredText(STRING_APP, bounds, TITLE_SIZE, ACTIVE_TEXT_COLOR);

    y += bounds.height;
    Vector2 player_selector_position = {WINDOW_WIDTH / 4 - BUTTON_WIDTH / 2, y};
    UpdatePlayerSelector(STRING_BLACK, 0, player_selector_position, black_player_type);
    player_selector_position.x += WINDOW_WIDTH / 2;
    UpdatePlayerSelector(STRING_WHITE, 1, player_selector_position, white_player_type);

    y += 3 * BUTTON_HEIGHT + 2 * BUTTON_GAP;
    Vector2 start_button_position = {WINDOW_WIDTH / 2 - BUTTON_WIDTH / 2,
                                     y + (WINDOW_HEIGHT - y) / 2 - BUTTON_HEIGHT / 2};
    if (UpdateButton(STRING_PLAY_GAME, start_button_position, TEXT_SIZE, false))
    {
        in_game = true;
        has_last_move = false;
        black_elapsed_seconds = 0.0;
        white_elapsed_seconds = 0.0;
        last_timer_update_time = GetTime();

        InitializeGame(game);
    }
}

/**
 * @brief Updates the turn timer, keeping track of how much time each player has spent on their turns.
 */
static void UpdateTurnTimer()
{
    const double now = GetTime();
    const double elapsed = now - last_timer_update_time;
    last_timer_update_time = now;

    GamePhase phase = GetGamePhase(game);
    if (phase == GAME_BLACK_PLAYS)
        black_elapsed_seconds += elapsed;
    else if (phase == GAME_WHITE_PLAYS)
        white_elapsed_seconds += elapsed;
}

/**
 * @brief Draws a square (disk) on the board at the given position with the appropriate color for the given square type.
 *
 * @param square The type of the square to draw.
 * @param center The center position of the square.
 * @param radius The radius of the square.

 */
static void DrawDisk(GameSquare square, Vector2 center, float radius)
{
    if (square == SQUARE_BLACK)
    {
        DrawCircleV(center, radius, BLACK_SQUARE_COLOR);
        DrawCircleLinesV(center, radius, BLACK_SQUARE_OUTLINE_COLOR);
    }
    else if (square == SQUARE_WHITE)
    {
        DrawCircleV(center, radius, WHITE_SQUARE_COLOR);
        DrawCircleLinesV(center, radius, WHITE_SQUARE_OUTLINE_COLOR);
    }
}

static void DrawScoreRow(float y,
                         GameSquare disk,
                         uint32_t score,
                         double elapsed_seconds,
                         Color text_color,
                         const char *player_name)
{
    // First row
    DrawCenteredText(player_name,
                     {SCORE_CARD_NAME_X, y - TEXT_SIZE / 2, SCORE_CARD_NAME_WIDTH, TEXT_SIZE},
                     TEXT_SIZE,
                     text_color);

    // Second row
    DrawDisk(disk, {SCORE_CARD_DISK_X, y + 1.3f * TEXT_SIZE}, SCORE_CARD_DISK_RADIUS);

    DrawCenteredText(TextFormat("%u", score),
                     {SCORE_CARD_SCORE_X, y + 1.3f * TEXT_SIZE - TEXT_SIZE / 2, SCORE_CARD_SCORE_WIDTH, TEXT_SIZE},
                     TEXT_SIZE,
                     text_color);

    uint32_t elapsed_time = (uint32_t)elapsed_seconds;
    DrawCenteredText(TextFormat("%02d:%02d", elapsed_time / 60, elapsed_time % 60),
                     {SCORE_CARD_TIME_X, y + 1.3f * TEXT_SIZE - TEXT_SIZE / 2, SCORE_CARD_TIME_WIDTH, TEXT_SIZE},
                     TEXT_SIZE,
                     text_color);
}

/**
 * @brief Updates and draws the game UI elements.
 */
static void UpdateGameUI()
{
    // Get current game phase and flags
    GamePhase phase = GetGamePhase(game);

    bool black_turn = phase == GAME_BLACK_PLAYS;
    bool white_turn = phase == GAME_WHITE_PLAYS;
    bool game_over = !black_turn && !white_turn;

    bool human_turn = ((black_turn && black_player_type == PLAYER_HUMAN) ||
                       (white_turn && white_player_type == PLAYER_HUMAN));

    // Get hovered cell
    Vector2 mouse = GetMousePosition();
    GamePosition hovered_cell = {(int32_t)((mouse.x - BOARD_X) / CELL_SIZE),
                                 (int32_t)((mouse.y - BOARD_Y) / CELL_SIZE)};
    bool hovered_is_valid = false;

    // Draw cells and disks
    for (int32_t y = 0; y < BOARD_SIZE; ++y)
    {
        for (int32_t x = 0; x < BOARD_SIZE; ++x)
        {
            Rectangle cell = {BOARD_X + x * CELL_SIZE, BOARD_Y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            DrawRectangleRec(cell, BOARD_CELL_COLOR);
            DrawRectangleLinesEx(cell, 1.0f, BOARD_LINE_COLOR);

            // Highlight valid moves
            bool is_valid_move = IsValidMove(game, {x, y});
            bool highlight_cell = human_turn && is_valid_move;

            if (highlight_cell)
            {
                bool this_hovered = hovered_cell.x == x && hovered_cell.y == y;
                Color hint_color = this_hovered ? Color{120, 180, 120, 200} : Color{80, 140, 80, 100};
                DrawRectangleRec(cell, hint_color);

                if (this_hovered)
                    hovered_is_valid = true;
            }

            // Highlight last move
            if (has_last_move && last_move.x == x && last_move.y == y)
            {
                Rectangle last_move_outline = {cell.x + 4.0f, cell.y + 4.0f,
                                               cell.width - 8.0f, cell.height - 8.0f};
                DrawRectangleLinesEx(last_move_outline, 4.0f, LAST_MOVE_COLOR);
            }

            // Draw disks
            GameSquare square = GetGameSquare(game, {x, y});
            if (square != SQUARE_EMPTY)
            {
                const float screen_x = cell.x + CELL_SIZE * 0.5f;
                const float screen_y = cell.y + CELL_SIZE * 0.5f;
                const float radius = CELL_SIZE * 0.38f;
                DrawDisk(square, {screen_x, screen_y}, radius);
            }
        }
    }

    if (hovered_is_valid)
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

    // Update turn timer
    UpdateTurnTimer();

    // Handle human click
    if (hovered_is_valid && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (PlayMove(game, hovered_cell))
        {
            last_move = hovered_cell;
            has_last_move = true;
        }
    }

    // Status card
    const Rectangle status_card_bounds = {PANEL_LEFT + PANEL_PADDING, STATUS_CARD_TOP,
                                          PANEL_WIDTH - PANEL_PADDING * 2, STATUS_CARD_HEIGHT};
    DrawRectangleRounded(status_card_bounds, 0.16f, 8, BUTTON_COLOR);
    DrawRectangleRoundedLinesEx(status_card_bounds, 0.16f, 8, 2.0f, BUTTON_BORDER_COLOR);

    const char *status_text = PHASE_LABELS[phase];
    DrawCenteredText(status_text, status_card_bounds, TEXT_SIZE, ACTIVE_TEXT_COLOR);

    // Score card
    const Rectangle score_card_bounds = {PANEL_LEFT + PANEL_PADDING, SCORE_CARD_TOP,
                                         PANEL_WIDTH - PANEL_PADDING * 2, SCORE_CARD_HEIGHT};
    DrawRectangleRounded(score_card_bounds, 0.16f, 8, BUTTON_COLOR);
    DrawRectangleRoundedLinesEx(score_card_bounds, 0.16f, 8, 2.0f, BUTTON_BORDER_COLOR);

    const uint32_t black_score = GetGameScore(game, SQUARE_BLACK);
    const uint32_t white_score = GetGameScore(game, SQUARE_WHITE);

    const Color black_row_color = black_turn ? ACTIVE_TEXT_COLOR : MUTED_TEXT_COLOR;
    const Color white_row_color = white_turn ? ACTIVE_TEXT_COLOR : MUTED_TEXT_COLOR;

    const char *black_player_name = (black_player_type == PLAYER_HUMAN) ? STRING_HUMAN : GetAgentName(0);
    const char *white_player_name = (white_player_type == PLAYER_HUMAN) ? STRING_HUMAN : GetAgentName(1);

    DrawScoreRow(SCORE_CARD_ROW1_Y,
                 SQUARE_BLACK,
                 black_score,
                 black_elapsed_seconds,
                 black_row_color,
                 black_player_name);

    // White row
    DrawScoreRow(SCORE_CARD_ROW2_Y,
                 SQUARE_WHITE,
                 white_score,
                 white_elapsed_seconds,
                 white_row_color, white_player_name);

    // Restart/Try again? button
    if (human_turn || game_over)
    {
        const char *button_text;
        if (!game_over)
            button_text = STRING_LEAVE_GAME;
        else
            button_text = STRING_PLAY_AGAIN;

        Vector2 button_position = {PANEL_LEFT + (PANEL_WIDTH - BUTTON_WIDTH) / 2, CONTROL_BUTTON_TOP};
        if (UpdateButton(button_text, button_position, TEXT_SIZE))
        {
            in_game = false;

            return;
        }
    }

    // Stop request
    if (IsKeyDown(KEY_ESCAPE))
        stop_requested = true;
}

/**
 * @brief Updates the UI for the current frame, drawing either the start menu or the game UI depending on the current state of the application.
 */
static void UpdateUIFrame()
{
    if (WindowShouldClose())
    {
        close_requested = true;
        stop_requested = true;
    }
    else
    {
        // Toggle fullscreen with F11 key
        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();

        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        BeginDrawing();

        DrawBackground();

        if (!in_game)
            UpdateStartMenuUI();
        else
            UpdateGameUI();

        EndDrawing();
    }
}

/**
 * @brief Function that the agents will call to check if a stop has been requested by the game engine. This function also ensures that the UI is updated at least every 0.1 seconds while an agent is running, so that the UI remains responsive and doesn't freeze while waiting for the agent to make a move.
 *
 * @return true if a stop has been requested, false otherwise.
 */
static bool StopRequested()
{
    // Update the UI at least every 0.1 seconds while an agent is running, to keep it responsive.
    const double now = GetTime();
    const double delta_time = now - last_stop_requested_ui_update_time;

    if (delta_time >= 0.1)
    {
        last_stop_requested_ui_update_time = now;

        UpdateUIFrame();
    }

    return stop_requested;
}

/**
 * @brief Plays the AI's move, calling the appropriate agent's get_next_move function based on whose turn it is, and updating the game state accordingly.
 *
 * @return true if the AI successfully played a move, false otherwise.
 */
static bool PlayAIMove()
{
    double start_time = GetTime();

    GamePhase phase = GetGamePhase(game);
    GamePosition move = {-1, -1};
    if (phase == GAME_BLACK_PLAYS)
        move = agent_instances[0].descriptor->get_next_move(agent_instances[0].state, game, StopRequested);
    else if (phase == GAME_WHITE_PLAYS)
        move = agent_instances[1].descriptor->get_next_move(agent_instances[1].state, game, StopRequested);

    // Ensure AI move is played for at least 1.5 seconds to give the user time to see the AI's move
    while ((GetTime() - start_time) < 1.25)
        UpdateUIFrame();

    if (PlayMove(game, move))
    {
        last_move = move;
        has_last_move = true;

        return true;
    }

    const char *side = (GetGamePhase(game) == GAME_BLACK_PLAYS) ? STRING_BLACK : STRING_WHITE;
    std::cerr << "error: " << side << " AI failed to play a valid move!" << std::endl;

    return false;
}

bool UpdateUI()
{
    UpdateUIFrame();

    if (in_game)
    {
        GamePhase phase = GetGamePhase(game);

        bool ai_turn = (phase == GAME_BLACK_PLAYS && black_player_type == PLAYER_AI) ||
                       (phase == GAME_WHITE_PLAYS && white_player_type == PLAYER_AI);

        if (ai_turn)
        {
            stop_requested = false;

            agent_running = true;
            PlayAIMove();
            agent_running = false;
        }
    }

    return !close_requested;
}
