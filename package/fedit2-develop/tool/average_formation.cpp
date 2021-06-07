
#include <rcsc/formation/formation.h>
#include <rcsc/formation/formation_dt.h>

#include <iostream>
#include <fstream>

using namespace rcsc;
using namespace rcsc::formation;

class AverageConfGenerator {
private:
    Formation::Ptr M_base_formation;
    Formation::Ptr M_target_formation;
public:

private:
    Formation::Ptr openFormation( const char * filepath );

    bool openBaseFormation( const char * filepath );
    bool openTargetFormation( const char * filepath );

public:

    bool generate( const char * base_filepath,
                   const char * target_filepath );
};

/*-------------------------------------------------------------------*/
/*!

 */
Formation::Ptr
AverageConfGenerator::openFormation( const char * filepath )
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
    if ( ! ptr->read( fin ) )
    {
        return Formation::Ptr();
    }
    fin.close();

    return ptr;

}

/*-------------------------------------------------------------------*/
/*!

 */
bool
AverageConfGenerator::openBaseFormation( const char * filepath )
{
    M_base_formation = openFormation( filepath );

    if ( ! M_base_formation )
    {
        std::cerr << "Could not open the base formation ["
                  << filepath << "]" << std::endl;
        return false;
    }



    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
AverageConfGenerator::openTargetFormation( const char * filepath )
{
    M_target_formation = openFormation( filepath );

    if ( ! M_target_formation )
    {
        std::cerr << "Could not open the target formation ["
                  << filepath << "]" << std::endl;
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
AverageConfGenerator::generate( const char * base_filepath,
                                const char * target_filepath )
{

    if ( ! openBaseFormation( base_filepath )
         || ! openTargetFormation( target_filepath ) )
    {
        return false;
    }

    SampleDataSet::ConstPtr samples = M_base_formation-> samples();
    if ( ! samples )
    {
        std::cerr << "Could not get the sample data of base formation."
                  << std::endl;
        return false;
    }

    Formation::Ptr average_formation( new FormationDT() );

    //
    // set role info
    //
    for ( int unum = 1; unum <= 11; ++unum )
    {
        int symmetry_unum = 0;
        if ( M_base_formation->isSideType( unum ) )
        {
            symmetry_unum = -1;
        }
        else if ( M_base_formation->isCenterType( unum ) )
        {
            symmetry_unum = 0;
        }
        else if ( M_base_formation->isSymmetryType( unum ) )
        {
            symmetry_unum = M_base_formation->getSymmetryNumber( unum );
        }

        average_formation->updateRole( unum,
                                       symmetry_unum,
                                       M_base_formation->getRoleName( unum ) );
        average_formation->updateRoleType( unum,
                                           M_base_formation->roleType( unum ).type() );
        average_formation->updateMarker( unum,
                                         M_base_formation->isMarker( unum ),
                                         M_base_formation->isSetPlayMarker( unum ) );
    }

    //
    // create new sample data set
    //

    SampleDataSet::Ptr average_samples( new SampleDataSet() );

    for ( SampleDataSet::DataCont::const_iterator it = samples->dataCont().begin();
          it != samples->dataCont().end();
          ++it )
    {
        SampleData new_data;
        new_data.ball_ = it->ball_;

        M_target_formation->getPositions( it->ball_, new_data.players_ );

        for ( int unum = 1; unum <= 11; ++unum )
        {
            new_data.players_[unum-1] += it->players_[unum-1];
            new_data.players_[unum-1] *= 0.5;
        }

        average_samples->addData( *average_formation,
                                  new_data,
                                  false );
    }


    //
    // generate formation
    //
    average_formation->setSamples( average_samples );
    average_formation->train();

    average_formation->print( std::cout );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
static
void
usage( const char * prog )
{
    std::cerr << prog << " base.conf target.conf" << std::endl;
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

    AverageConfGenerator g;
    g.generate( argv[1], argv[2] );

    return 0;
}
