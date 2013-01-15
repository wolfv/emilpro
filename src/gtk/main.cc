#include <gtkmm.h>

#include <model.hh>
#include <utils.hh>

using namespace emilpro;

class EmilProGui
{
public:
	EmilProGui()
	{
	}

	void init(int argc, char **argv)
	{
		m_app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

		m_builder = Gtk::Builder::create_from_file("/home/ska/projects/emilpro/src/gtk/emilpro.glade");

		Gtk::ImageMenuItem *fileOpenItem;
		m_builder->get_widget("file_menu_open", fileOpenItem);

		panic_if (!fileOpenItem,
				"Can't get file_menu_open");
		fileOpenItem->signal_activate().connect(sigc::mem_fun(*this, &EmilProGui::onFileOpen));
	}

	void run()
	{
		Gtk::Window * mainWindow = NULL;
		m_builder->get_widget("main_window", mainWindow);

		m_app->run(*mainWindow);
	}

protected:
	void onFileOpen()
	{
		Gtk::FileChooserDialog *openFile = NULL;
		m_builder->get_widget("file_chooser", openFile);

		panic_if (!openFile,
				"Open file dialogue does not exist");

		int v = openFile->run();

		if (v == Gtk::RESPONSE_ACCEPT) {
			size_t sz;

			void *data = read_file(&sz, "%s", openFile->get_filename().c_str());

			if (data) {
				Model::instance().addData(data, sz);
			}
		}

		openFile->hide();
	}

private:
	Glib::RefPtr<Gtk::Application> m_app;
	Glib::RefPtr<Gtk::Builder> m_builder;
};

int main(int argc, char **argv)
{
	EmilProGui gui;

	gui.init(argc, argv);

	gui.run();

	return 0;
}