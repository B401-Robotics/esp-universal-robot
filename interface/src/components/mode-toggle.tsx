import { Moon, Sun } from 'lucide-react'

import { Button } from '@/components/ui/button'
import { useTheme } from '@/components/theme-provider'

export function ModeToggle() {
	const { setTheme } = useTheme()

	return (
		<Button
			variant="outline"
			size="icon"
			onClick={() => setTheme(!document.documentElement.classList.contains('dark') ? 'dark' : 'light')}>
			{document.documentElement.classList.contains('dark') ? (
				<Sun className="h-[1.2rem] w-[1.2rem] rotate-0 scale-100 transition-all dark:-rotate-90 dark:scale-0" />
			) : (
				<Moon className="h-[1.2rem] w-[1.2rem] rotate-90 scale-0 transition-all dark:rotate-0 dark:scale-100" />
			)}
		</Button>
	)
}
