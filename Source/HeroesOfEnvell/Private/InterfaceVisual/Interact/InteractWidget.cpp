#include "InterfaceVisual/Interact/InteractWidget.h"
#include "Components/TextBlock.h"

void UInteractWidget::SetActionText(FText ButtonText)
{
	if (ActionText)
	{
		ActionText->SetText(ButtonText);
	}
}