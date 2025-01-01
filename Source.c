#include <gtk/gtk.h>  
#define MAX_RECORDS 100  
typedef struct {
    GtkWidget* entry_amount;
    GtkWidget* combo_category;
    GtkWidget* calendar;
    GtkWidget* entry_note;
    GtkWidget* list_store;
    GtkWidget* tatol_w;
}
AppWidgets;

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
const char* categories[] = { u8"早餐", u8"午餐", u8"晚餐", u8"飲品", u8"點心", u8"酒類",     u8"交通", u8"購物", u8"娛樂", u8"日用品", u8"房租", NULL };
int category_count = sizeof(categories) / sizeof(categories[0]) - 1;
static void
update_total_label(GtkWidget* label) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), u8"總金額：$%.2f", total_amount);
    gtk_label_set_text(GTK_LABEL(label), buffer);
}// 主程式入口 
int main(int argc, char** argv) {
    GtkApplication* app;
    int status;
    app = gtk_application_new("com.example.accounting", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}