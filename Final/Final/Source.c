static void add_record(GtkWidget* widget, gpointer data) {
    AppWidgets* widgets = (AppWidgets*)data;

    const gchar* amount_text = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets->entry_amount)));
    const gchar* note = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(widgets->entry_note)));

    float amount = atof(amount_text);
    if (amount <= 0) {
        g_print(u8"請輸入有效金額。\n");
        return;
    }

    GDateTime* datetime = gtk_calendar_get_date(GTK_CALENDAR(widgets->calendar));
    guint year = g_date_time_get_year(datetime);
    guint month = g_date_time_get_month(datetime);
    guint day = g_date_time_get_day_of_month(datetime);
    g_date_time_unref(datetime);

    gint category_index = gtk_drop_down_get_selected(GTK_DROP_DOWN(widgets->combo_category));
    if (category_index < 0 || category_index >= category_count) {
        g_print(u8"請選擇有效的類別。\n");
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

    g_print(u8"記錄已新增：[%s] %s - $%.2f\n", new_record.date, new_record.category, new_record.amount);

    update_total_label(widgets->tatol_w);
}