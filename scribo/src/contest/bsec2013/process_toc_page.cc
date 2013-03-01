#include <iostream>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgba.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/qt/rgb32.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pdf/load.hh>
#include <mln/io/pdf/get_header.hh>
#include <mln/data/convert.hh>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <mln/debug/filename.hh>
#include <QString>
#include <QStringList>
#include <QDebug>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " <doc.pdf> <page>" << std::endl;
    return 1;
  }

  using namespace poppler;
  using namespace mln;

  // Load document
  poppler::document *pdf = poppler::document::load_from_file(argv[1]);
  if (pdf == 0)
  {
    std::cerr << "Error: Cannot load PDF " << argv[1] << std::endl;
    abort();
  }



  // Read page
  poppler::page* p = pdf->create_page(atoi(argv[2]));
  std::vector<char> byte_array = p->text().to_utf8();
  if (byte_array.size() > 0)
  {
    QString text = QString::fromUtf8((const char *)&byte_array[0]);

    // Remove '.' and '-' (usually used in table of contents between
    // title and page number.
    text = text.replace('.', ' ');
    text = text.replace('-', ' ');
    text = text.replace('*', ' ');

    QStringList entries = text.split('\n');

    qDebug() << entries;

    qDebug();


    // PASS 1 - Remove useless spaces.
    for (int i = 0; i < entries.size(); ++i)
      entries[i] = entries[i].simplified();


    // PASS 2 - Merge lines.
    for (int i = 0; i < entries.size();)
    {
      bool removed = false;
      QString& entry = entries[i];

      // Skip Table of Contents title.
      if (entry.contains("table of contents", Qt::CaseInsensitive))
      {
	++i;
	continue;
      }

      // Remove empty lines
      if (entry.isEmpty() || entry == "") // FIXME: There is a bug!
					  // Some empty strings seems
					  // to pass this test!
					  // (100_book_subset/pdfs/aidstothebible01popeuoft.pdf
					  // page10)
      {
	entries.removeAt(i);
	continue;
      }

      // Move single numbers to previous lines.
      qDebug() << "Checking " << entry;
      if (entry.contains(QRegExp("^[0-9mcxilv]+$", Qt::CaseInsensitive)))
      {
	if ((i - 1) > 0)
	{
	  //qDebug() << "MERGING BACK " << entry << " TO " << entries[i - 1];

	  entries[i - 1].append(" " + entry);
	  entries.removeAt(i);
	  removed = true;
	}
      }
      // Trying to merge lines corresponding to entries on several
      // lines.
      else if (! entry.contains(QRegExp(" [0-9mcxilv]+$", Qt::CaseInsensitive)))
	if ((i + 1) < entries.size())
	{
	  //qDebug() << "MERGING FORWARD " << entries[i + 1] << " TO " << entry;
	  entry.append(" " + entries[i + 1].simplified());
	  entries.removeAt(i + 1);
	  removed = true;
	  //entries[i + 1].clear();
	}

      if (!removed)
	++i;
    }

    // PASS 3 - Grouping page numbers if there is space in between.
    // Remove multiple invalid page numbers like "14 VI".
    for (int i = 0; i < entries.size(); ++i)
    {
      QString& entry = entries[i];

      // Garbage page numbers. Keep the first one.
      if (entry.contains(QRegExp(" [0-9]+ [mcxilv]+$", Qt::CaseInsensitive))
	|| entry.contains(QRegExp(" [mcxilv]+ [0-9]+$", Qt::CaseInsensitive)))
      {
	QStringList words = entry.split(' ');
	QString new_entry = "";
	for (int j = 0; j < words.size() - 2; ++j)
	  new_entry += words[j] + " ";
	new_entry += words[words.size() - 1];
	entry = new_entry;
      }
      else if (entry.contains(QRegExp(" [mcxilv]+ [mcxilv]+$", Qt::CaseInsensitive)))
      {
	QStringList words = entry.split(' ');
	QString new_entry = "";
	for (int j = 0; j < words.size() - 2; ++j)
	  new_entry += words[j] + " ";
	new_entry += words[words.size() - 2] + words[words.size() - 1];
	entry = new_entry;
      }
      else if (entry.contains(QRegExp(" [0-9]+ [0-9]+$", Qt::CaseInsensitive)))
      { // Same case as previous one except that we check if the page number exists!
	QStringList words = entry.split(' ');
	QString new_entry = "";
	for (int j = 0; j < words.size() - 2; ++j)
	  new_entry += words[j] + " ";

	int page = QString(words[words.size() - 2] + words[words.size() - 1]).toInt();

	if (page > pdf->pages())
	  new_entry += words[words.size() - 2]; // Keep first page number
	else
	  new_entry += words[words.size() - 2] + words[words.size() - 1]; // Merging both.
	entry = new_entry;
      }

    }

    qDebug() << entries;
  }


  // io::pdf::pdf_header header =
  //   io::pdf::get_header(argv[1]);

  // std::cout << header.page_count << std::endl;
  // util::array<image2d<value::rgb8> > arr;
  // io::pdf::load(arr, argv[1], 0, 10);

  // for (unsigned i = 0; i < arr.size(); ++i)
  //   io::ppm::save(arr[i], mln::debug::filename("page.ppm"));

  // util::array<int> pages;
  // pages.append(1);
  // pages.append(9);

  // arr.clear();
  // io::pdf::load(arr, argv[1], pages);

  // for (unsigned i = 0; i < arr.size(); ++i)
  //   io::ppm::save(arr[i], mln::debug::filename("pages.ppm"));


  //pima.save("page.png", "PNG", 300);



}
