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
const char* categories[] = { u8"���\", u8"���\", u8"���\", u8"���~", u8"�I��", u8"�s��",     u8"��q", u8"�ʪ�", u8"�T��", u8"��Ϋ~", u8"�Я�", NULL };
int category_count = sizeof(categories) / sizeof(categories[0]) - 1;
static void
update_total_label(GtkWidget* label) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), u8"�`���B�G$%.2f", total_amount);
    gtk_label_set_text(GTK_LABEL(label), buffer);
}// �D�{���J�f 
int main(int argc, char** argv) {
    GtkApplication* app;
    int status;
    app = gtk_application_new("com.example.accounting", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}