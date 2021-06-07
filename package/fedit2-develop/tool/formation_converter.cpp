
#include <rcsc/formation/formation.h>
#include <rcsc/formation/formation_dt.h>

#include <iostream>
#include <fstream>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
Formation::Ptr
openFormation( const char * filepath )
{
    std::ifstream fin( filepath );

    if ( ! fin )
    {
        return Formation::Ptr();
    }

    Formation::Ptr ptr = Formation::create( fin );

    if ( ! ptr )
    {
        return Formation::Ptr();
    }

    fin.seekg( 0 );
    if ( ! ptr->readOld( fin ) )
    //if ( ! ptr->readCSV( fin ) )
    {
        return Formation::Ptr();
    }
    fin.close();

    return ptr;

}
/*-------------------------------------------------------------------*/
/*!

 */
static
void
usage( const char * prog )
{
    std::cerr << prog << " FORMATION.CONF" << std::endl;
}

/*-------------------------------------------------------------------*/
/*!

 */
int
main( int argc, char ** argv )
{
    if ( argc < 2 )
    {
        usage( argv[0] );
        return 1;
    }

    std::string filepath = argv[1];

    Formation::Ptr f = openFormation( filepath.c_str() );

    if ( ! f )
    {
        std::cerr << "Could not open the formation " << filepath << std::endl;
        return 1;
    }

    //std::string new_filepath = filepath + ".csv";
    //std::ofstream fout( new_filepath.c_str() );
    //f->printCSV( fout );
    //fout.close();
    f->printCSV( std::cout );

    return 0;
}
