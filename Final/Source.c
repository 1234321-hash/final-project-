#include <gtk/gtk.h>

#define MAX_RECORDS 100

typedef struct {
    GtkWidget* entry_amount;
    GtkWidget* combo_category;
    GtkWidget* calendar;
    GtkWidget* entry_note;
    GtkWidget* list_store;
    GtkWidget* tatol_w;
} AppWidgets;

typedef struct {
    char category[15];
    float w;
    float amount;
    char date[11];
    char note[50];
} Record;

Record records[MAX_RECORDS];
int record_count = 0;
float total_amount = 0.0;

const char* categories[] = {
    u8"���\", u8"���\", u8"���\", u8"���~", u8"�I��", u8"�s��",
    u8"��q", u8"�ʪ�", u8"�T��", u8"��Ϋ~", u8"�Я�", NULL
};
int category_count = sizeof(categories) / sizeof(categories[0]) - 1;

static void update_total_label(GtkWidget* label) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), u8"�`���B�G$%.2f", total_amount);
    gtk_label_set_text(GTK_LABEL(label), buffer);
}

static void add_record(GtkWidget* widget, gpointer data) {
    AppWidgets* widgets = (AppWidgets*)data;

    const gchar* amount_text = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets->entry_amount)));
    const gchar* note = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets->entry_note)));

    float amount = atof(amount_text);
    if (amount <= 0) {
        g_print(u8"�п�J���Ī��B�C\n");
        return;
    }

    GDateTime* datetime = gtk_calendar_get_date(GTK_CALENDAR(widgets->calendar));
    guint year = g_date_time_get_year(datetime);
    guint month = g_date_time_get_month(datetime);
    guint day = g_date_time_get_day_of_month(datetime);
    g_date_time_unref(datetime);

    gint category_index = gtk_drop_down_get_selected(GTK_DROP_DOWN(widgets->combo_category));
    if (category_index < 0 || category_index >= category_count) {
        g_print(u8"�п�ܦ��Ī����O�C\n");
        return;
    }

    Record new_record;
    snprintf(new_record.category, sizeof(new_record.category), "%s", categories[category_index]);
    snprintf(new_record.date, sizeof(new_record.date), "%4d-%02d-%02d", year, month, day);
    new_record.amount = amount;
    snprintf(new_record.note, sizeof(new_record.note), "%s", note);
    new_record.w = amount;

    records[record_count] = new_record;
    record_count++;
    total_amount += amount;


    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(widgets->list_store), &iter);
    gtk_list_store_set(GTK_LIST_STORE(widgets->list_store), &iter,
        0, new_record.date,
        1, new_record.category,
        2, amount_text,
        3, new_record.note,
        4, total_amount,
        -1);

    g_print(u8"�O���w�s�W�G[%s] %s - $%.2f\n", new_record.date, new_record.category, new_record.amount);

    update_total_label(widgets->tatol_w);
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid, * button, * scrolled_window, * tree_view;
    GtkCellRenderer* renderer;
    GtkTreeViewColumn* column;
    GtkListStore* store;

    AppWidgets* widgets = g_malloc(sizeof(AppWidgets));

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), u8"�O�b�{��");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    widgets->entry_amount = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_amount), u8"���B");
    gtk_grid_attach(GTK_GRID(grid), widgets->entry_amount, 0, 0, 1, 1);

    widgets->entry_note = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_note), u8"�Ƶ�");
    gtk_grid_attach(GTK_GRID(grid), widgets->entry_note, 1, 0, 1, 1);

    widgets->calendar = gtk_calendar_new();
    gtk_grid_attach(GTK_GRID(grid), widgets->calendar, 0, 1, 2, 1);

    GtkStringList* list = gtk_string_list_new((const char* const*)categories);
    widgets->combo_category = gtk_drop_down_new(G_LIST_MODEL(list), NULL);
    gtk_grid_attach(GTK_GRID(grid), widgets->combo_category, 2, 0, 1, 1);
    // �s�W���s
    button = gtk_button_new_with_label(u8"�s�W�O��");
    gtk_grid_attach(GTK_GRID(grid), button, 3, 0, 1, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(add_record), widgets);

    // �`�p����
    widgets->tatol_w = gtk_label_new(u8"�`���B�G$0.00");
    gtk_grid_attach(GTK_GRID(grid), widgets->tatol_w, 0, 4, 4, 1);

    // �M����ܰ�
    scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 4, 1);

    // ������ܰO��
    store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT);
    widgets->list_store = GTK_WIDGET(store);

    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), tree_view);

    // �K�[����C
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(u8"���", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // �K�[���O�C
    column = gtk_tree_view_column_new_with_attributes(u8"���O", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // �K�[���B�C
    column = gtk_tree_view_column_new_with_attributes(u8"���B", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // �K�[�Ƶ��C
    column = gtk_tree_view_column_new_with_attributes(u8"�Ƶ�", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    // �K�[�`�B�C
    column = gtk_tree_view_column_new_with_attributes(u8"�`�p", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    gtk_widget_show(window);
}

// �D�{���J�f
int main(int argc, char** argv) {
    GtkApplication* app;
    int status;

    app = gtk_application_new("com.example.accounting", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}