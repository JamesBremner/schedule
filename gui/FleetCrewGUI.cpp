

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include  <nana/gui/msgbox.hpp>
#include <nana/gui/widgets/textbox.hpp>



class cVehicleType
{
public:
    cVehicleType()
    {

    }
    cVehicleType( const std::string& name, int crew )
        : myName( name)
        , myCrew( crew )
    {

    }
    std::string Name()
    {
        return myName;
    }
    int Crew()
    {
        return myCrew;
    }
private:
    std::string myName;
    int myCrew;
};



class cVehicle
{
public:
    cVehicle( const std::string& type,
              const std::string& plate )
        : myType( type )
        , myPlate( plate )
    {

    }
    std::string Type()
    {
        return myType;
    }
    std::string Plate()
    {
        return myPlate;
    }
private:
    std::string myType;
    std::string myPlate;
};

class cPerson
{
public:
    cPerson( const std::string& name )
        : myName( name )
    {

    }
    std::string Name()
    {
        return myName;
    }
private:
    std::string myName;
};

class cFleet
{
public:
    void Add( const cVehicle& v )
    {
        myFleet.push_back( v );
    }

    void NewType( nana::form& fm )
    {
        nana::inputbox::text name("Name", "");
        nana::inputbox::integer crew("Number of crew",3,1,10,1);
        nana::inputbox inbox( fm, "New Vehicle Type" );

        if( inbox.show( name, crew ) )
        {
            cVehicleType vt;
            if( FindType( name.value(), vt ))
            {
                nana::msgbox msg("Already have this vehicle type");
                msg.show();
            }
            else
            {
                myTypeVector.push_back( cVehicleType(
                                            name.value(),
                                            crew.value() ) );
            }
        }
    }
    void NewVehicle( nana::form& fm )
    {
        std::vector<std::string> type_names;
        for( auto& t : myTypeVector )
        {
            type_names.push_back( t.Name() );
        }
        nana::inputbox::text type("Type", type_names );
        char plate_default[] { 65 + myFleet.size(), 0 };
        nana::inputbox::text plate("Plate", std::string(plate_default));
        nana::inputbox inbox( fm, "Vehicle Type to add" );
        if( inbox.show(type, plate ) )
        {
            myFleet.push_back( cVehicle( type.value(), plate.value() ));
        }

    }
    void NewPerson( nana::form& fm )
    {
        nana::inputbox::text name("Name", "");
        nana::inputbox inbox( fm, "New Person" );
        if( inbox.show( name ) )
        {
            if( FindPerson( name.value() ))
            {
                nana::msgbox msg("Already have person with this name");
                msg.show();
            }
            else
            {
                myPersonVector.push_back( cPerson( name.value() ));
            }
        }

    }
    void Display( nana::textbox& fleet_text )
    {
        fleet_text.select( true );
        fleet_text.del();
        fleet_text.append(std::to_string( myFleet.size())+ " Vehicles:\n", false);
        for( auto& v : myFleet )
        {
            fleet_text.append( v.Plate()+" "+v.Type()+"\n", false );
        }
        fleet_text.append("\n" + std::to_string( myPersonVector.size())+ " People:\n", false);
        for( auto& p : myPersonVector )
        {
            fleet_text.append( p.Name() + "\n", false );
        }
        if( myAssign.size() )
        {
            fleet_text.append("\nSchedule:\n",false);
            for( auto& a : myAssign )
            {
                fleet_text.append(
                    a.second.Name()+" in "+a.first.Type()+" plate "+a.first.Plate()+" "+"\n",
                    false);
            }
        }
    }

    bool Schedule()
    {
        myAssign.clear();

        std::vector< cPerson >::iterator ItPerson = myPersonVector.begin();

        // loop over vehicles
        for( auto& v : myFleet )
        {
            cVehicleType vt;
            if( ! FindType( v.Type(), vt ) )
                throw std::runtime_error("Vehicle type error");
            // loop over people needed by vehicle type
            for( int kp = 0; kp < vt.Crew(); kp++ )
            {
                if( ItPerson == myPersonVector.end() )
                {
                    nana::msgbox msg("Not enough people to crew the fleet");
                    msg.show();
                    myAssign.clear();
                    return false;
                }
                myAssign.push_back( std::pair< cVehicle, cPerson >( v, *ItPerson ) );

                ItPerson++;
            }
        }
        return true;
    }

private:

    std::vector< cVehicle > myFleet;
    std::vector< cVehicleType > myTypeVector;
    std::vector< cPerson > myPersonVector;
    std::vector< std::pair< cVehicle, cPerson > > myAssign;

    bool FindType( const std::string& type_name,
                   cVehicleType& type )
    {
        for( auto& t : myTypeVector )
            if( t.Name() == type_name )
            {
                type = t;
                return true;
            }
        return false;
    }
    bool FindPerson( const std::string& type )
    {
        for( auto& p : myPersonVector )
            if( p.Name() == type )
                return true;
        return false;
    }
};

int main()
{
    cFleet theFleet;

    // construct application form
    nana::form fm( nana::rectangle( 100,100, 250, 450 ));
    nana::textbox fleet_text( fm, nana::rectangle( 10,150, 250, 350 ));

    nana::button new_type_button( fm, nana::rectangle( 10, 10, 150, 20 ) );
    new_type_button.caption("Add new type of vehicle");
    new_type_button.events().click([&]
    {
        theFleet.NewType( fm );
    });
    nana::button new_vehicle_button( fm, nana::rectangle( 10, 40, 150, 20 ) );
    new_vehicle_button.caption("Add new vehicle to Fleet");
    new_vehicle_button.events().click([&]
    {
        theFleet.NewVehicle( fm );
        theFleet.Display( fleet_text );
    });
    nana::button new_person_button( fm, nana::rectangle( 10, 70, 150, 20 ) );
    new_person_button.caption("Add new person");
    new_person_button.events().click([&]
    {
        theFleet.NewPerson( fm );
        theFleet.Display( fleet_text );
    });
    nana::button schedule_button( fm, nana::rectangle( 10, 100, 100, 20 ) );
    schedule_button.caption("SCHEDULE");
    schedule_button.events().click([&]
    {
        theFleet.Schedule();
        theFleet.Display( fleet_text );
    });

    // show & run
    fm.show();
    nana::exec();

}
